#include <string>
#include "folder.h"
#include <tinyxml.h>
#include <boost\filesystem.hpp>

namespace syncro {

using namespace boost::filesystem;
using boost::shared_ptr;

using namespace std;

CFolder::CFolder( const string& insPath ) : m_sPath( insPath ) {
	path oPath( m_sPath );
	m_sFolderName = oPath.filename();
	if( !is_directory( oPath ) )
		return;
	for( directory_iterator pItem( oPath ); pItem != directory_iterator(); pItem++ ) { 
		if( is_directory( pItem->status() ) ) {
			m_oChildren.push_back( boost::shared_ptr<CFolder>( new CFolder( pItem->path().directory_string() ) ) );
		} else {
			//m_oFiles.push_back( pItem->path().directory_string() );
			m_oFiles.push_back( pItem->filename() );
		}
	}
}

CFolder::~CFolder( ) {

}

};		//end namespace syncro