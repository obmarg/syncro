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
			nCount++;
			m_aFolderInfo.push_back( sFolderInfo( nCount, pItem->path().native_directory_string() ) );
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

};		//end namespace syncro