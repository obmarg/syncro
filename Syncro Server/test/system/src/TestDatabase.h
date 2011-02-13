/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	This file defines TestDatabase, a class responsible for creating 
	a syncro database for use in the system test

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

#ifndef SYNCRO_TEST_DATABASE_H_
#define SYNCRO_TEST_DATABASE_H_

#include "../../Syncro Server/src/include/SyncroDB.h"
#include <string>

namespace syncro {
namespace test {

class TestDatabase
{
public:
	TestDatabase( const std::string& database );

	void Create();
private:
	std::string		m_filename;
};

}	// namespace test
}	// namespace syncro

#endif