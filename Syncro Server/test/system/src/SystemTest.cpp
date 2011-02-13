/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	This file is responsible for the implementation of the server system
	test

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

#include "SystemTest.h"

namespace syncro {
namespace test {

CPPUNIT_TEST_SUITE_REGISTRATION( SystemTest );

client::ConnectionDetails	SystemTest::m_serverDetails;
bool						SystemTest::m_serverPrepared;

void SystemTest::setUp()
{

}

void SystemTest::tearDown()
{

}

void SystemTest::Connect()
{
	using client::Connection;

	CPPUNIT_ASSERT_NO_THROW(
		m_connection.reset(
			new Connection( m_serverDetails )
			)
		);
}

void SystemTest::ConnectionTest()
{
	Connect();
	CPPUNIT_ASSERT( m_connection );

	//TODO: Check connection details or something?
}

void SystemTest::FolderListTest()
{
	Connect();

	syncro::FolderList list;

	CPPUNIT_ASSERT_NO_THROW(
		m_connection->GetFolderList( list )
		);

	if( m_serverPrepared )
	{
		//TODO: Add testing for readable and writable
		CPPUNIT_ASSERT( list.size() == 3 );

		CPPUNIT_ASSERT( list[0].Id == 1 );
		CPPUNIT_ASSERT( list[0].Name == "Test1" );
		CPPUNIT_ASSERT( 
			list[0].Path == "Test1/" || list[0].Path == "Test1\\"
			);
		CPPUNIT_ASSERT( list[0].UploadPrefix == "" );

		CPPUNIT_ASSERT( list[1].Id == 65539 );
		CPPUNIT_ASSERT( list[1].Name == "Test2" );
		CPPUNIT_ASSERT( 
			list[1].Path == "Test2/" || list[1].Path == "Test2\\"
			);
		CPPUNIT_ASSERT( list[1].UploadPrefix == "" );

		CPPUNIT_ASSERT( list[2].Id == 1280012 );
		CPPUNIT_ASSERT( list[2].Name == "Test3" );
		CPPUNIT_ASSERT( 
			list[2].Path == "Test3/" || list[2].Path == "Test3\\"
			);
		CPPUNIT_ASSERT( list[2].UploadPrefix == "" );
	}
}

void SystemTest::FolderContentsTest()
{
	Connect();

}

}	// namespace test
}	// namespace syncro