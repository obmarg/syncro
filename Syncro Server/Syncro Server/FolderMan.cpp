#include "FolderMan.h"
#include "Folder.h"
#include "BinaryDataRequest.h"
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
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

CFolderMan::CFolderMan( Database::TPointer inpDB ) : m_pDB(inpDB) {
	Database::ResultSet oRS = inpDB->run( "SELECT ID,Name,Path FROM Folders" );
	//TODO: make sure boost foreach actually works on this, might need some more things added to resultset first (value_type etc. maybe)
	foreach( Database::Row& oRow, oRS ) {
		path oPath( oRow["Path"] );
		if( !is_directory( oPath ) )
			throw std::runtime_error( "Invalid path read from DB in CFolderMan constructor" );
		//TODO: Do something with the name as well
		m_folders.push_back( FolderInfo( boost::lexical_cast<int>( oRow["ID"] ) , oPath.native_directory_string() ) );
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
	return boost::shared_ptr<CFolder>( new CFolder( FindFolder(nFolderID).Name ) );
	//TODO: handle exception here or elsewhere?
}

std::string
CFolderMan::GetFileName(int nFolderId,const std::string& fileName) {
	std::string rv = FindFolder(nFolderId).Name;
	char aLastChar = *( rv.rbegin() ) ;
	if( (aLastChar != '\\') && (aLastChar != '/') )
		rv += "/";
	rv += fileName;
	return rv;
}

bool 
CFolderMan::IncomingFile( 
	const CBinaryDataRequest& fileData, 
	IncomingFileDetails& details 
	)
{
	const FolderInfo& folderInfo = FindFolder( fileData.GetFolderId() );
	std::string destFileName = folderInfo.Name;
	if( fileData.IsOneShot() )
	{
		//If one shot, then we need to upload to a temporary folder

		//TODO: First, have the temporary path configurable 
		//		(for now it's just temp in run dir)
		//		Then: Extract configuration stuff to a seperate class in kode...
		destFileName = "temp/";
		boost::filesystem::path path( destFileName );
		if( !boost::filesystem::exists( path ) )
		{
			boost::filesystem::create_directories( path );
		}
	}
	destFileName += fileData.GetFilename();
	path destFile( destFileName );
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
	if( !exists( destFile ) )
	{
		return true;
	}
	int nFileSize = fileData.GetFileSize();
	if( nFileSize != -1 ) 
	{
		if( file_size( destFile ) != nFileSize )
			return true;
	}
	//TODO: SHould i just throw an exception here?
	return false;
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
		//		then move the file to a prfix of that id (or just id for
		//		filename)  to save overwriting things accidentally.
	}
}

};		//end namespace syncro
