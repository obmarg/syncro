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

#include "SystemTest.h"
#include "TestDatabase.h"
#include <libsyncro/connection.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <boost/program_options.hpp>
#include <fstream>

namespace po = boost::program_options;

int main( int argc, char** argv )
{
	using namespace syncro::test;

	po::options_description desc( "Allowed options" );
	desc.add_options()
		( "help,h", "produce help message" )
		( "prepared", "server is using prepared database" )
		( "server,s", po::value< std::string >(), "server address/ip" )
		( "port,p", po::value< uint32_t >(), "server port" )
		( "file,f", po::value< std::string >(), "file to transfer" )
		( 
			"createdatabase,c", 
			po::value< std::string >(), 
			"create a test db with the given filename" 
			);

	po::variables_map vm;
	try {
		po::store( po::parse_command_line( argc, argv, desc ), vm );
		po::notify( vm );
	}
	catch( const std::exception& e )
	{
		std::cout << "Error: " << e.what() << "\n\n" 
			<< desc << "\n";
		return 1;
	}

	if( vm.count("help") )
	{
		std::cout << desc << "\n";
		return 1;
	}

	if( vm.count("createdatabase") )
	{
		TestDatabase db( vm["createdatabase"].as<std::string>() );
		db.Create();
		//TODO: Catch exceptions, check return value etc.
		return 0;
	}

	if( vm.count( "prepared" ) )
	{
		SystemTest::SetServerPrepared( true );
	}
	if( vm.count( "file" ) )
	{
		SystemTest::SetTransferTestFilename( vm["file"].as<std::string>() );
	}

	syncro::client::ConnectionDetails details;
	if( vm.count( "server" ) )
	{
		details.SetHostname( vm["server"].as<std::string>() );
	}
	else
	{
		std::cout << "No hostname supplied, using localhost" << std::endl;
		details.SetHostname( "localhost" );
	}
	if( vm.count( "port" ) )
	{
		details.SetPort( vm["port"].as< uint32_t >() );
	}

	SystemTest::SetConnectionDetails( details );

	std::string tests = "All Tests";
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = 
		CppUnit::TestFactoryRegistry::getRegistry( tests );
	
	std::ofstream resultFile("result.xml");
	// Specify XML output and inform the test runner of this format (optional)
        CppUnit::XmlOutputter* outputter =
		new CppUnit::XmlOutputter(
				&runner.result(),
			       	resultFile,
			       	std::string( "ISO-8859-1" )
				);
    	runner.setOutputter(outputter);

	runner.addTest( registry.makeTest() );
	bool success = runner.run();

	return !success;
}
