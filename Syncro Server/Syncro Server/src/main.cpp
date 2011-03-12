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

#include "SyncroServer.h"
#include "SyncroDB.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <string>

#ifdef LINUX
#include <sys/types.h>
#include <unistd.h>
#endif

namespace po = boost::program_options;

int main( int argc, char** argv )
{
	using namespace syncro;

	po::options_description desc( "Allowed options" );
	desc.add_options()
		( "help,h", "produce help message" )
		( "port,p", po::value<unsigned int>(), "port to run server on" )
		( "nobroadcast,b", "disable udb broadcast receiver" )
		( 
			"daemon", 
			po::value< std::string >()->implicit_value( "" ),
		       	"run as daemon (linux only)" 
			)
		( 
			"database,d", 
			po::value<std::string>(), 
			"use specified database file" 
			);

	po::variables_map vm;
	bool parsedOk = true;
	try {
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
		std::cout << desc;
		return 1;
	}
	if( vm.count( "database" ) )
	{
		std::string databaseFile( vm[ "database" ].as< std::string >() );
		std::cout << "Using database file: " << databaseFile << "\n";
		SyncroDB::SetDefaultFilename( databaseFile );
	}
	unsigned int port = 0;
	if( vm.count("port") )
	{
		port = vm[ "port" ].as< unsigned int >();
		std::cout << "Using port: " << port << "\n";
	}
	bool broadcast = true;
	if( vm.count( "nobroadcast" ) )
	{
		broadcast = false;
	}
	if( vm.count( "daemon" ) )
	{
#ifndef LINUX
		//throw std::logic_error( "Daemon not implemented on win32" )
		std::cout << "Daemon not implemented on win32\n";
		return 1;
#else
		if( daemon( true, false ) == -1 )
		{
			std::cout << "Could not run as daemon\n";
			return 1;	
		}
		std::string pidFilename = 
			vm[ "daemon" ].as< std::string >();
		pid_t pid = getpid();
		if( pidFilename != "" )
		{
			std::ofstream pidFile( pidFilename.c_str() );
			pidFile << pid;
			pidFile.close();
		}
#endif
	}

	boost::shared_ptr< syncro::SyncroServer > server;
	server.reset( 
		new syncro::SyncroServer( port, broadcast )
		);
	bool fOK = server->Run();
	return fOK ? 0 : 1;
}
