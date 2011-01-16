// SyncroAdmin.cpp : Defines the entry point for the console application.
//

#include <libsyncro/connection.h>
#include <libsyncro/scanner.h>
#include <kode/net.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	using namespace syncro::client;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("scan,s", "scan for servers")
		("addfolder,a", po::value< std::string >(), "add folder")
		("uploadfile,u", po::value< std::string >(), "upload file")
		("folderid,f", po::value< int >(), "folder id to use for upload/download")
		("oneshot,o", "upload file as one shot file");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    

	if( vm.count("help") )
	{
		std::cout << desc << "\n";
		return 1;
	}

	if( vm.count("scan") )
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

		if( vm.count("addfolder") )
		{
			std::string folderName = vm["addfolder"].as<std::string>();
			boost::filesystem::path path( folderName );
			if( !boost::filesystem::is_directory( path ) )
				std::cout << "Error: Path supplied to addfolder is not a directory\n";
			else
			{
				Connection::StringMap params;
				params.insert( std::make_pair( "path", folderName ) );
				conn.SendAdminCommand( "AddFolder", params );
			}
		}
		if( vm.count("uploadfile") )
		{
			if( !vm.count("folderid") )
			{
				throw std::runtime_error( "Please provide a folder id to upload to" );
			}
			std::string filename = vm["uploadfile"].as<std::string>();
			boost::filesystem::path path(filename);
			
			conn.UploadFile(
				UploadFileDetails()
					.SetFolderId( vm["folderid"].as<int>() )
					.SetLocalPath( filename )
					.SetRemotePath( path.filename() )
					.SetOneShot( vm.count("oneshot") > 0 )
				);

		}


	/*	syncro::FolderList list;

		conn.GetFolderList( list );

		conn.SendAdminCommand("AddFolder","C:\\temp\\");

		int i = 0;
		i += 1;*/
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

