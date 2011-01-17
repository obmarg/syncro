#include "FolderMan.h"
#include "Folder.h"
#include "BinaryDataRequest.h"
#include <libsyncro/stringutils.h>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <string>

namespace syncro {

using namespace std;
using namespace boost::filesystem;
using namespace kode::db;

class UploadFinishDetails {
public:
	UploadFinishDetails(
		const std::string& inFilename,
		const bool inOneShot,
		const int inFolderId,
		const std::string& inFolderPath,
		const std::string& inLocalPath
		) :
	fileName( inFilename ),
	oneShot( inOneShot ),
	folderId( inFolderId ),
	folderPath( inFolderPath ),
	localPath( inLocalPath )
	{

	}
	std::string fileName;
	bool oneShot;
	unsigned int folderId;
	std::string folderPath;
	std::string localPath;
};

// For now, DownloadFinishDetails is the same as upload finish
class DownloadFinishDetails : public UploadFinishDetails
{
public:
	DownloadFinishDetails(
		const std::string& inFilename,
		const bool inOneShot,
		const int inFolderId,
		const std::string& inFolderPath,
		const std::string& inLocalPath,
		const int inFileId=-1
		) :
	UploadFinishDetails(
		inFilename,
		inOneShot,
		inFolderId,
		inFolderPath,
		inLocalPath
		),
	fileId( inFileId )
	{ }
	int fileId;
};

CFolderMan::CFolderMan( Database::TPointer inpDB ) : m_pDB(inpDB) {
	Database::ResultSet oRS = inpDB->run( "SELECT ID,Name,Path,UploadPrefix FROM Folders" );
	//TODO: make sure boost foreach actually works on this, might need some more things added to resultset first (value_type etc. maybe)
	foreach( Database::Row& oRow, oRS ) {
		path oPath( oRow["Path"] );
		if( !is_directory( oPath ) )
			throw std::runtime_error( "Invalid path read from DB in CFolderMan constructor" );
		//TODO: Do something with the name as well
		m_folders.push_back( 
			FolderInfo( 
				boost::lexical_cast<int>( oRow["ID"] ) , 
				oRow["Name"],
				oPath.native_directory_string(),
				oRow["UploadPrefix"]
				) 
			);
		if( ( *m_folders.back().Path.rbegin() ) != '/'
			&& ( *m_folders.back().Path.rbegin() ) != '\\' )
		{
			//TODO: Replace this with real path seperator for current platform
			m_folders.back().Name += "/";
		}
	}
}

CFolderMan::~CFolderMan() {

}

const FolderInfo& CFolderMan::FindFolder( int nFolderId ) {
	foreach( const FolderInfo& oInfo, m_folders ) {
		if( oInfo.Id == nFolderId ) {
			return oInfo;
		}
	}
	throw std::range_error("Invalid folder ID passed to CFolderMan");
}


boost::shared_ptr<CFolder> 
CFolderMan::GetFolder( int nFolderID ) {
	const FolderInfo& info = FindFolder(nFolderID);
	boost::shared_ptr<CFolder> rv( 
		new CFolder( info.Path, info.Name ) 
		);
	//TODO: handle exception here or elsewhere?

	if( !m_listOneShots )
	{
		m_listOneShots = m_pDB->prepare(
			"SELECT FileName,LocalPath FROM files "
			"WHERE OneShot=1 AND FolderID=?"
			);
	}
	kode::db::AutoReset reset( m_listOneShots );
	m_listOneShots->Bind( 1, nFolderID );
	while( m_listOneShots->GetNextRow() )
	{
		std::string localPath = m_listOneShots->GetColumn< std::string >( 1 );
		std::string name =  m_listOneShots->GetColumn< std::string >( 0 );
		path filePath( localPath );
		if( exists( filePath ) )
		{
			uint32_t size =
				boost::numeric_cast<uint32_t>( file_size( filePath ) );
			rv->AddFile( name, size );
		}
	}
	return rv;
}

bool
CFolderMan::FileRequested(
	const CBinaryDataRequest& requestData, 
	FileTransferDetails& details
	) 
{
	std::string rv = 
		FindFolder( requestData.GetFolderId() ).Path;

	char aLastChar = *( rv.rbegin() ) ;
	if( (aLastChar != '\\') && (aLastChar != '/') )
		rv += "/";
	rv += requestData.GetFilename();
	bool oneShot = false;
	int oneShotId = -1;
	if( !exists( path(rv) ) )
	{
		if( !m_findOneShot )
		{
			m_findOneShot = m_pDB->prepare(
				"SELECT ID,LocalPath,OneShot FROM files "
				"WHERE FolderID=? AND FileName=?"
				);
		}
		kode::db::AutoReset reset( m_findOneShot );
		m_findOneShot->Bind(1, requestData.GetFolderId() );
		m_findOneShot->Bind(2, requestData.GetFilename() );
		//TODO: Need to check database in case this is a one shot file.
		if( m_findOneShot->GetNextRow() )
		{
			//We've found a one shot file, sweet...
			oneShotId = m_findOneShot->GetColumn< int >( 0 );
			rv = m_findOneShot->GetColumn< std::string >( 1 );
			oneShot = m_findOneShot->GetColumn< int >( 2 ) != 0;
		} 
		else
		{
			return false;
		}
	}
	details.m_filename = rv;
	//TODO: Add support for folder path in here. for now doesn't matter
	//		can possibly remove the parameter if it turns out just the
	//		name is good enough
	//TODO: Also need to seperate UploadFInishDetails and DownloadFInishDetails
	DownloadFinishDetailsPtr finishDetails(
		new DownloadFinishDetails(
			requestData.GetFilename(),
			oneShot,
			requestData.GetFolderId(),
			"",
			rv,
			oneShotId
			)
		);
	details.m_callback = boost::bind(
		&CFolderMan::FileDownloadFinished,
		this,
		finishDetails
		);
	return true;
}

bool 
CFolderMan::IncomingFile( 
	const CBinaryDataRequest& fileData, 
	FileTransferDetails& details 
	)
{
	const FolderInfo& folderInfo = FindFolder( fileData.GetFolderId() );
	std::string destFileName = folderInfo.Path;
	if( fileData.IsOneShot() )
	{
		//If one shot, then we need to upload to a temporary folder

		//TODO: First, have the temporary path configurable 
		//		(for now it's just temp in run dir)
		//		Then: Extract configuration stuff to a seperate class in kode...
		destFileName = "temp/";
	}
	else if( !folderInfo.UploadPrefix.empty() )
	{
		std::string prefix = folderInfo.UploadPrefix;
		syncro::utils::ReplaceStringVars( prefix );
		//TODO: Need to ensure that the prefix has a trailing slash
		destFileName += prefix;
	}
	boost::filesystem::path path( destFileName );
	if( !boost::filesystem::exists( path ) )
	{
		boost::filesystem::create_directories( path );
	}
	destFileName += fileData.GetFilename();
	boost::filesystem::path destFile( destFileName );
	details.m_filename = destFile.native_file_string();
	//TODO: Add support for folder path in here. for now doesn't matter
	//		can possibly remove the parameter if it turns out just the
	//		name is good enough
	UploadFinishDetailsPtr finishDetails(
		new UploadFinishDetails(
			fileData.GetFilename(),
			fileData.IsOneShot(),
			fileData.GetFolderId(),
			"",
			destFileName
			)
		);
	details.m_callback = boost::bind(
		&CFolderMan::FileUploadFinished,
		this,
		finishDetails
		);
	int64_t nFileSize = fileData.GetFileSize();
	if( exists(destFile) && nFileSize != -1 ) 
	{
		if( file_size( destFile ) != nFileSize )
		{
			// File size doesn't match, definetly download again.
			return true;
		}
	}
	//
	// Check the database to see if this has been uploaded already
	//
	if( !m_checkUploadHistory ) 
	{
		m_checkUploadHistory = m_pDB->prepare(
			"SELECT ID,ActualFilename FROM UploadHistory "
			"WHERE FolderID=? AND Filename=?"
			);
	}
	bool accept = true;
	kode::db::AutoReset reset( m_checkUploadHistory );
	m_checkUploadHistory->Bind( 1, fileData.GetFolderId() );
	m_checkUploadHistory->Bind( 2, fileData.GetFilename() );
	while( m_checkUploadHistory->GetNextRow() )
	{
		boost::filesystem::path otherFile( 
			m_checkUploadHistory->GetColumn<std::string>( 1 )
			);
		if( exists( otherFile ) )
		{
			if( file_size( otherFile ) == nFileSize )
			{
				accept = false;
			}
		}
	}
	//TODO: SHould i just throw an exception here?
	return accept;
}

void CFolderMan::FileUploadFinished( UploadFinishDetailsPtr details )
{
	if( details->oneShot )
	{
		//If we're a one shot file, we need to add a database entry
		if( !m_addOneShot )
		{
			m_addOneShot = m_pDB->prepare(
				"INSERT INTO Files "
				"(Filename,FolderPath,LocalPath,FolderId,OneShot) "
				"VALUES (?, ?, ?, ?, 1);"
				);
		}
		kode::db::AutoReset reset( m_addOneShot );
		m_addOneShot->Bind( 1, details->fileName );
		m_addOneShot->Bind( 2, details->folderPath );
		m_addOneShot->Bind( 3, details->localPath );
		m_addOneShot->Bind( 4, details->folderId );
		m_addOneShot->GetNextRow();
		//TODO: Maybe after commiting that row, query it, and get the ID
		//		then move the file to a prefix of that id (or just id for
		//		filename)  to save overwriting things accidentally.
	}
	else
	{
		if( !m_addToUploadHistory )
		{
			m_addToUploadHistory = m_pDB->prepare(
				"INSERT INTO UploadHistory "
				"(Filename, FolderId, ActualFilename) "
				"VALUES( ?, ?, ?);"
				);
		}
		kode::db::AutoReset reset( m_addToUploadHistory );
		m_addToUploadHistory->Bind( 1, details->fileName );
		m_addToUploadHistory->Bind( 2, details->folderId );
		m_addToUploadHistory->Bind( 3, details->localPath );
		m_addToUploadHistory->GetNextRow();
	}
	
	//TODO: At some point, would be good to clear out the old uploaded files database,
	//		based on clients no longer attempting to upload the file.
	//		Could keep a list of files requested upload, then on destruction
	//		delete anything that's not in the list...
}

void CFolderMan::FileDownloadFinished( DownloadFinishDetailsPtr details )
{
	if( details->oneShot )
	{
		if( !m_delOneShot )
		{
			m_delOneShot = m_pDB->prepare(
				"DELETE FROM Files WHERE OneShot=1 AND ID=?"
				);
		}
		kode::db::AutoReset reset( m_delOneShot );
		m_delOneShot->Bind( 1, details->fileId );
		m_delOneShot->GetNextRow();
	}
}

};		//end namespace syncro
