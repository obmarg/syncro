#include <string>
#include "Folder.h"
#include <tinyxml.h>
#include <boost/filesystem.hpp>

namespace syncro {

using namespace boost::filesystem;
using boost::shared_ptr;

using namespace std;

CFolder::CFolder( const string& insPath ) : m_sPath( insPath ) {
	using boost::filesystem::path;
	using boost::filesystem::file_size;

	path oPath( insPath );
	m_sFolderName = oPath.filename();
	if( !is_directory( oPath ) )
		return;
	for( directory_iterator pItem( oPath ); pItem != directory_iterator(); pItem++ ) { 
		if( is_directory( pItem->status() ) ) {
			m_oChildren.push_back( boost::shared_ptr<CFolder>( new CFolder( pItem->path().directory_string() ) ) );
		} else {
			//m_oFiles.push_back( pItem->path().directory_string() );
			
			m_oFiles.push_back( sFileData(pItem->filename(), (const unsigned int)file_size( pItem->path() ) ) );
		}
	}
}

CFolder::~CFolder( ) {

}

};		//end namespace syncro
