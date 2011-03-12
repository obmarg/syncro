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

// SyncroAdmin.cpp : Defines the entry point for the console application.
//

#include <libsyncro/connection.h>
#include <libsyncro/scanner.h>
#include <kode/net.h>
#include <kode/utils.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iostream>

namespace po = boost::program_options;

int main( int argc, char* argv[] )
{
	using namespace syncro::client;

	po::options_description desc( "Allowed options" );
	desc.add_options()
	( "help,h", "produce help message" )
	( "scan,s", "scan for servers" )
	( "list,l", "list folders" )
	( "addfolder,a", po::value< std::string >(), "add folder" )
	( "delfolder,d", po::value< int >(), "delete specified folder id" )
	( "uploadfile,u", po::value< std::string >(), "upload file" )
	( "contents,c", po::value< int >(), "get contents of specified folder" )
	( "folderid,f", po::value< int >(), "folder id to use for upload/download" )
	( "oneshot,o", "upload file as one shot file" );

	po::variables_map vm;
	bool parsedOk = true;
	try 
	{
		po::store( po::parse_command_line( argc, argv, desc ), vm );
		po::notify( vm );
	}
	catch( const std::exception& ex )
	{
		parsedOk = false;
		std::cout << "Invalid parameters\n";
		std::cout << "What: " << ex.what() << "\n\n";
	}

	if( !parsedOk || vm.count( "help" ) )
	{
		std::cout << desc << "\n";
		return 1;
	}

	if( vm.count( "scan" ) )
	{
		try
		{
			Scanner scanner;
			scanner.Scan( 5000 );
			BOOST_FOREACH( Scanner::ServerDetailsPtr details, scanner.Servers() )
			{
				std::cout << "Found Server: " << details->Name() << "\n";
			}
		}
		catch( const kode::net::NetworkException& ex )
		{
			std::cout << "Exception: " << ex.what() << "\n";
		}
		return 0;
	}

	try
	{
		Connection conn(
		    ConnectionDetails()
		    .SetUsername( "admin" )
		    .SetPassword( "password" )
		    .SetHostname( "localhost" )
		);

		if( vm.count( "addfolder" ) )
		{
			std::string folderName = vm["addfolder"].as<std::string>();
			boost::filesystem::path path( folderName );
			if( !boost::filesystem::is_directory( path ) )
				std::cout << "Error: Path supplied to addfolder is not a directory\n";
			else
			{
				Connection::StringMap params;
				params.insert( std::make_pair( "path", folderName ) );
				params.insert( std::make_pair( "name", path.native_file_string() ) );
				conn.SendAdminCommand( "AddFolder", params );
			}
		}
	   	else if( vm.count( "delfolder" ) )
		{
			int folderId = vm[ "delfolder" ].as< int >();
			std::string folderIdStr = boost::lexical_cast< std::string >( folderId );
			Connection::StringMap params;
			params.insert( std::make_pair( "id", folderIdStr ) );
			conn.SendAdminCommand( "DelFolder", params );
		}
		if( vm.count( "list" ) )
		{
			syncro::FolderList list;
			conn.GetFolderList( list );
			
			int nNum = 0;
			std::cout << "Folders:\n";
			BOOST_FOREACH( const syncro::FolderInfo& folder, list )
			{
				std::cout << folder.Id << ": " << folder.Name 
					<< " ( " << folder.Path << " )\n";
				nNum++;
			}
		}
		if( vm.count( "contents" ) )
		{
			syncro::FileList list;
			conn.GetFolderContents( vm[ "contents" ].as< int >(), list );
			std::cout << "File List:\n";
			BOOST_FOREACH( const syncro::FileInfo& file, list )
			{
				std::cout << file.name << std::endl;
			}
		}
		if( vm.count( "uploadfile" ) )
		{
			if( !vm.count( "folderid" ) )
			{
				throw std::runtime_error( "Please provide a folder id to upload to" );
			}
			std::string filename = vm["uploadfile"].as<std::string>();
			boost::filesystem::path path( filename );

			conn.UploadFile(
			    FileTransferDetails()
			    .SetFolderId( vm["folderid"].as<int>() )
			    .SetLocalPath( filename )
			    .SetRemotePath( path.filename() )
			    .SetOneShot( vm.count( "oneshot" ) > 0 )
			);

		}
	}
	catch( const kode::net::NetworkException& ex )
	{
		std::cout << "Network Exception: " << ex.what() << "\n";
	}
	catch( const std::exception& ex )
	{
		std::cout << "Exception: " << ex.what() << "\n";
	}
	return 0;
}

