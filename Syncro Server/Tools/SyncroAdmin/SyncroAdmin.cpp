// SyncroAdmin.cpp : Defines the entry point for the console application.
//

#include <libsyncro/connection.h>
#include <kode/net.h>
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	using namespace syncro::client;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("addfolder,a", po::value< std::string >(), "add folder");
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    

	if( vm.count("help") )
	{
		std::cout << desc << "\n";
		return 1;
	}

	try 
	{
		Connection conn(
			ConnectionDetails()
				.SetUsername( "grambo" )
				.SetPassword( "password" )
				.SetHostname( "localhost" )
			);

		if( vm.count("addfolder") )
		{
			conn.SendAdminCommand( "AddFolder", vm["addfolder"].as<std::string>() );
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
	return 0;
}

