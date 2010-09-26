#include "FolderMan.h"
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

CFolderMan::CFolderMan( const string& insRoot ) : m_sRoot( insRoot ) {
	path oPath( m_sRoot );
	if( !is_directory( oPath ) )
		throw std::exception( "Invalid path" );
	int nCount = 0;
	for( directory_iterator pItem( oPath ); pItem != directory_iterator(); pItem++ ) { 
		if( is_directory( pItem->status() ) ) {
			m_oFolders.push_back( pItem->path().native_directory_string() );
		}
	}
}

CFolderMan::~CFolderMan() {

}


boost::shared_ptr<CFolder> 
CFolderMan::GetFolder( int nIndex ) {
	if( (nIndex < 0) || (nIndex > (int)m_oFolders.size()) )
		return boost::shared_ptr<CFolder>();
	return boost::shared_ptr<CFolder>( new CFolder( m_oFolders[ nIndex ] ) );
}