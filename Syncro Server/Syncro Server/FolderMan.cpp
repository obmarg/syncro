#include "FolderMan.h"
#include "BinaryDataRequest.h"
#include <boost/filesystem.hpp>

namespace syncro {

using namespace std;
using namespace boost::filesystem;
using namespace kode::db;

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

std::string 
CFolderMan::IncomingFile( const CBinaryDataRequest& fileData )
{
	const FolderInfo& folderInfo = FindFolder( fileData.GetFolderId() );
	std::string destFileName = folderInfo.Name + fileData.GetFilename();
	path destFile( destFileName );
	if( !exists( destFile ) )
		return destFile.native_file_string();
	int nFileSize = fileData.GetFileSize();
	if( nFileSize != -1 ) 
	{
		if( file_size( destFile ) != nFileSize )
			return destFile.native_file_string();
	}
	//TODO: SHould i just throw an exception here?
	return std::string();
}
};		//end namespace syncro
