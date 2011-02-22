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
#include <string>

namespace po = boost::program_options;

int main( int argc, char** argv )
{
	po::options_description desc( "Allowed options" );
	desc.add_options()
		( "help,h", "produce help message" )
		( "port,p", po::value<unsigned int>(), "port to run server on" )
		( 
			"database,d", 
			po::value<std::string>(), 
			"use specified database file" 
			);

	po::variables_map vm;
	po::store( po::parse_command_line( argc, argv, desc ), vm );
	po::notify( vm );

	if( vm.count( "help" ) )
	{
		std::cout << desc;
		return 1;
	}
	if( vm.count( "database" ) )
	{
		syncro::CSyncroDB::SetDefaultFilename( 
									vm[ "database" ].as< std::string >() 
									);
	}
	unsigned int port = 0;
	if( vm.count("port") )
	{
		port = vm[ "port" ].as< unsigned int >();
	}

	boost::shared_ptr< syncro::SyncroServer > server;
	server.reset( 
		new syncro::SyncroServer( port )
		);
	bool fOK = server->Run();
	return fOK ? 0 : 1;
}
