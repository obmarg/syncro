#include "FolderMan.h"
#include <boost/filesystem.hpp>

namespace syncro {

using namespace std;
using namespace boost::filesystem;

CFolderMan::CFolderMan( const string& insRoot ) : m_sRoot( insRoot ) {
	path oPath( m_sRoot );
	if( !is_directory( oPath ) )
		throw std::exception( "Invalid path" );
	int nCount = 0;
	for( directory_iterator pItem( oPath ); pItem != directory_iterator(); pItem++ ) { 
		if( is_directory( pItem->status() ) ) {
			m_aFolderInfo.push_back( sFolderInfo( nCount, pItem->path().native_directory_string() ) );
			nCount++;
		}
	}
}

CFolderMan::~CFolderMan() {

}


boost::shared_ptr<CFolder> 
CFolderMan::GetFolder( int nIndex ) {
	if( (nIndex < 0) || (nIndex > (int)m_aFolderInfo.size()) )
		return boost::shared_ptr<CFolder>();
	return boost::shared_ptr<CFolder>( new CFolder( m_aFolderInfo[ nIndex ].sFolderName ) );
}

std::string
CFolderMan::GetFileName(int nFolderIndex,const std::string& fileName) {
	if( m_aFolderInfo.empty() )
		return false;
	if( ( nFolderIndex < 0 ) || ( nFolderIndex > m_aFolderInfo.size() ) )
		return false;
	std::string rv = m_aFolderInfo[ nFolderIndex ].sFolderName;
	char aLastChar = *( rv.rbegin() ) ;
	if( (aLastChar != '\\') && (aLastChar != '/') )
		rv += "/";
	rv += fileName;
	return rv;
}

};		//end namespace syncro