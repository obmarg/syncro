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

client::ConnectionDetails SystemTest::m_serverDetails;

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

	//TODO: Need to make this actually check the output etc.
}

void SystemTest::FolderContentsTest()
{
	Connect();

	//TODO: Add some actual test code....
}

}	// namespace test
}	// namespace syncro