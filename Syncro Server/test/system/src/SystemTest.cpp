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
#include <libsyncro/comms.h>
#include <kode/hashutils.h>
#include <cryptopp/sha.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <time.h>

CPPUNIT_TEST_SUITE_REGISTRATION( SystemTest );

client::ConnectionDetails	SystemTest::m_serverDetails;
bool						SystemTest::m_serverPrepared;
std::string					SystemTest::m_transferTestFileName =
											"TransferTemp.txt";

static const std::string TRANSFER_FILE_CONTENTS = "Testing.txt";

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

void SystemTest::FileTransferTest()
{
	using namespace syncro::client;
	Connect();

	bool fileCreated = CreateTestFile( m_transferTestFileName );
	FileTransferDetails details;
	details
		.SetFolderId( 1 )
		.SetLocalPath( m_transferTestFileName )
		.SetOneShot( false )
		.SetRemotePath( m_transferTestFileName );

	CPPUNIT_ASSERT_NO_THROW_MESSAGE(
		"Upload file threw exception",
		m_connection->UploadFile( details ) 
		);

	FileList filesOnServer;
	CPPUNIT_ASSERT_NO_THROW_MESSAGE( 
		"Get File Contents threw Exception",
		m_connection->GetFolderContents( 1, filesOnServer ) 
		);

	bool foundFile = false;
	BOOST_FOREACH( const syncro::FileInfo& file, filesOnServer )
	{
		if( file.name == m_transferTestFileName )
		{
			foundFile = true;
			break;
		}
	}
	CPPUNIT_ASSERT_MESSAGE( "Could not find file after upload", foundFile );

	details.SetLocalPath( "temp.dat" );
	CPPUNIT_ASSERT_NO_THROW_MESSAGE(
		"File Download Threw Exception",
		m_connection->DownloadFile( details ) 
		);

	CPPUNIT_ASSERT( boost::filesystem::exists( "temp.dat" ) );

	kode::HashPtr originalHash = 
		kode::HashFile< CryptoPP::SHA >( m_transferTestFileName );
	kode::HashPtr downloadHash = 
		kode::HashFile< CryptoPP::SHA >( "temp.dat" );
	CPPUNIT_ASSERT_MESSAGE(
		"Downloaded file hash does not match original hash",
		(*originalHash) == (*downloadHash) 
		);

	boost::filesystem::remove( "temp.dat" );
	if( fileCreated )
	{
		boost::filesystem::remove( m_transferTestFileName );
	}
}

void SystemTest::FileModificationTimeTest()
{
	using namespace syncro::client;
	Connect();

	bool fileCreated = CreateTestFile( m_transferTestFileName );

	//
	// Modify the file time to be not just now
	//
	tm *timeStruct;
	time_t currentTime = time( 0 );
	timeStruct = localtime( &currentTime );
	//TODO: This isn't perfect.  Time may overflow.  but deal with
	//		that when not in rush
	if( timeStruct->tm_sec > 3 )
	{
		timeStruct->tm_min -= 2;
	}
	else
	{
		timeStruct->tm_min += 2;
	}
	time_t newTime = mktime( timeStruct );

	boost::filesystem::last_write_time( 
		m_transferTestFileName,
		newTime
		);

	CPPUNIT_ASSERT_EQUAL( 
		newTime,
		boost::filesystem::last_write_time( m_transferTestFileName )
		);

	//
	// First, upload our file
	//
	FileTransferDetails details;
	details
		.SetFolderId( 1 )
		.SetLocalPath( m_transferTestFileName )
		.SetOneShot( false )
		.SetRemotePath( m_transferTestFileName );

	CPPUNIT_ASSERT_NO_THROW_MESSAGE(
		"Upload file threw exception",
		m_connection->UploadFile( details ) 
		);

	details.SetLocalPath( "temp.dat" );
	CPPUNIT_ASSERT_NO_THROW_MESSAGE(
		"File Download Threw Exception",
		m_connection->DownloadFile( details ) 
		);

	CPPUNIT_ASSERT_EQUAL( 
		newTime,
		boost::filesystem::last_write_time( "temp.dat" )
		);

	boost::filesystem::remove( "temp.dat" );
}

bool SystemTest::CreateTestFile( const std::string& fileName )
{
	if( !boost::filesystem::exists( fileName ) )
	{
		std::ofstream fileOut( fileName.c_str() );
		fileOut << TRANSFER_FILE_CONTENTS;
		fileOut.close();
		return true;
	}

	return false;
}