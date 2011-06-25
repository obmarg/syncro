/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	This file is responsible for the definition of the server system
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

#include <libsyncro/connection.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>

namespace syncro {

namespace client {

class Connection;

}	// namespace client
}	// namespace syncro

using namespace syncro;

class SystemTest : public CppUnit::TestFixture
{
public:
	SystemTest() : CppUnit::TestFixture() {};

	CPPUNIT_TEST_SUITE( SystemTest );

	CPPUNIT_TEST( ConnectionTest );
	CPPUNIT_TEST( FolderListTest );
	CPPUNIT_TEST( FolderContentsTest );
	CPPUNIT_TEST( FileTransferTest );
	CPPUNIT_TEST( FileModificationTimeTest );

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

public:
	//
	// Test functions
	//
	void ConnectionTest();
	void FolderListTest();
	void FolderContentsTest();
	void FileTransferTest();
	void FileModificationTimeTest();

	//
	// Utility functions
	//
	bool CreateTestFile( const std::string& fileName );

public:
	static void SetConnectionDetails( 
		const client::ConnectionDetails& details 
		)
	{ m_serverDetails = details; }

	static void SetServerPrepared( bool prepared )
	{ m_serverPrepared = prepared; }

	static void SetTransferTestFilename( const std::string& name )
	{ m_transferTestFileName = name; }

private:
	static client::ConnectionDetails	m_serverDetails;
	static bool							m_serverPrepared;
	static std::string					m_transferTestFileName;

private:
	void Connect();

private:
	boost::shared_ptr< client::Connection > m_connection;
	bool m_ok;
};

