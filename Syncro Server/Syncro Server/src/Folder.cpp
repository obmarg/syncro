#include <string>
#include "Folder.h"
#include <tinyxml.h>
#include <boost/filesystem.hpp>

namespace syncro {

using namespace boost::filesystem;
using boost::shared_ptr;

using namespace std;

CFolder::CFolder( const string& insPath, const string& name ) : 
m_sFolderName( name )
{
	using boost::filesystem::path;
	using boost::filesystem::file_size;

	path oPath( insPath );
	m_sPath = oPath.filename();
	if( !is_directory( oPath ) )
		return;
	for( directory_iterator pItem( oPath ); pItem != directory_iterator(); pItem++ ) 
	{ 
		if( is_directory( pItem->status() ) ) 
		{
			boost::shared_ptr<CFolder> folder( 
				new CFolder( 
					pItem->path().directory_string(),
					pItem->path().directory_string() 
					) 
				);
			m_oChildren.push_back( folder );
		} 
		else 
		{
			//m_oFiles.push_back( pItem->path().directory_string() );
			
			m_oFiles.push_back( sFileData(pItem->filename(), (const unsigned int)file_size( pItem->path() ) ) );
		}
	}
}

CFolder::~CFolder( ) {

}

void
CFolder::AddFile( const std::string& name, unsigned int size )
{
	m_oFiles.push_back( sFileData( name, size ) );
}

};		//end namespace syncro
