/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	This file implements TestDatabase, a class responsible for creating 
	a syncro database for use in the system test

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

#include "TestDatabase.h"
#include <boost/filesystem.hpp>

namespace syncro {
namespace test {

static const std::string FOLDER_INSERT_PREFIX = 
	"INSERT INTO FOLDERS (ID,Name,Path,Writable,Readable,UploadPrefix) "
	"VALUES";

static const std::string FOLDER_INSERT_DATA1 = 
	"(1,'Test1','Test1/',0,0,'');";
static const std::string FOLDER_INSERT_DATA2 = 
	"(65539,'Test2','Test2/',1,0,'');";
static const std::string FOLDER_INSERT_DATA3 = 
	"(1280012,'Test3','Test3/',0,1,'');";

TestDatabase::TestDatabase( const std::string& file ) :
m_filename( file )
{

}

void TestDatabase::Create()
{
	boost::filesystem::remove( m_filename );

	SyncroDB::TPointer db( SyncroDB::OpenDB( m_filename ) );
	db->run( FOLDER_INSERT_PREFIX + FOLDER_INSERT_DATA1 );
	db->run( FOLDER_INSERT_PREFIX + FOLDER_INSERT_DATA2 );
	db->run( FOLDER_INSERT_PREFIX + FOLDER_INSERT_DATA3 );
}

}	// namespace test
}	// namespace syncro