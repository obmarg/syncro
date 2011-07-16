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

	This file defines the base class for databases
*/

#ifndef KODE_DB_DATABASE_H_
#define KODE_DB_DATABASE_H_

#include <boost/shared_ptr.hpp>

namespace kode {
namespace db {

// 
// Pre-declare objects
//
class Statement;
class BaseDatabase;

//
// Typedef shared pointers to objects
//
typedef boost::shared_ptr< Statement > StatementPtr;
typedef boost::shared_ptr< BaseDatabase > DatabasePtr;

//////////////////////////////////////////////////////////////////////////
//!
//!	\brief	The base class for databases
//!
//////////////////////////////////////////////////////////////////////////
class BaseDatabase
{
public:
	virtual ~BaseDatabase()
	{

	}

	//!
	//!	\brief	Runs an insert query
	//!
	//!	\param	query	The sql to run
	//!
	virtual void runInsert( const std::string& sql )=0;

	//!
	//!	\brief	Prepares a statement for use
	//!
	//!	\param	sql		The sql to prepare into a statement
	//!
	//!	\return		A pointer to a statement
	//!
	virtual StatementPtr prepare( const std::string& sql )=0;
};

}	// namespace db
}	// namespace kode

#endif	// KODE_DB_DATABASE_H_
