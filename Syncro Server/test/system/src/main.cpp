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
#include <libsyncro/connection.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main( int argc, char** argv )
{
	syncro::client::ConnectionDetails details;
	details.SetHostname( "localhost" );
	syncro::test::SystemTest::SetConnectionDetails( details );

	std::string tests = "All Tests";
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = 
		CppUnit::TestFactoryRegistry::getRegistry( tests );

	runner.addTest( registry.makeTest() );
	runner.run();

	return 0;
}