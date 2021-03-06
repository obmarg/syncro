/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include "Folder.h"
#include <boost/filesystem.hpp>

namespace syncro
{

using namespace boost::filesystem;
using boost::shared_ptr;

using namespace std;

Folder::Folder( const string& insPath, const string& name ) :
	m_sFolderName( name )
{
	using boost::filesystem::path;
	using boost::filesystem::file_size;

	path oPath( insPath );
	m_sPath = oPath.filename().string();
	if( !is_directory( oPath ) )
		return;
	for( directory_iterator pItem( oPath ); pItem != directory_iterator(); pItem++ )
	{
		if( is_directory( pItem->status() ) )
		{
			boost::shared_ptr<Folder> folder(
			    new Folder(
			        pItem->path().string(),
			        pItem->path().filename().string()
			    )
			);
			m_oChildren.push_back( folder );
		}
		else
		{
			//m_oFiles.push_back( pItem->path().directory_string() );

			m_oFiles.push_back( 
			        sFileData( 
			            pItem->path().filename().string(), 
			            ( const unsigned int )file_size( pItem->path() ) 
			            ) );
		}
	}
}

Folder::~Folder( )
{

}

void
Folder::AddFile( const std::string& name, unsigned int size )
{
	m_oFiles.push_back( sFileData( name, size ) );
}

};		//end namespace syncro
