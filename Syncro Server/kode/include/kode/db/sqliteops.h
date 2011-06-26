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

#ifndef KODE_DB_SQLITEOPS_H_
#define KODE_DB_SQLITEOPS_H_

#include "kode/db/utils.h"
#include <sqlite3.h>
#include <string>
#include <stdexcept>

namespace kode {
namespace db { 

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	Generic template for performing Bind/GetColumn operations
//!
//!			This version of the template does not have definitions, and
//!			should never be used, instead the specializations below
//!			should be used.
//!
///////////////////////////////////////////////////////////////////////////////
template<class DataType>
class SqliteOps
{
public:
	//!
	//!	\brief	Binds data to a statement
	//!
	//!	\param	statement	The statement to bind to
	//!	\param	index		The index of this parameter ( 1 based )
	//!	\param	data		The data to bind
	//!
	//!	\throws A SqlException on error
	//!
	static void Bind( sqlite3_stmt* statement, int index,  DataType data );

	//!
	//!	\brief	Gets a column from a statement
	//!
	//!	\param	statement	The statement to retreive from
	//!	\param	index		The index of this column ( 0 based )
	//!
	//!	\return	The data contained in the specified column
	//!
	//!	\throws A SqlException on error
	//!
	static DataType GetColumn( sqlite3_stmt* statement, int index );
};

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	SqliteOps specialization for std::string
//!
///////////////////////////////////////////////////////////////////////////////
template<>
class SqliteOps< std::string >
{
public:
	//!
	//!	\brief	Binds data to a statement
	//!
	//!	\param	statement	The statement to bind to
	//!	\param	index		The index of this parameter ( 1 based )
	//!	\param	data		The data to bind
	//!
	//!	\throws A SqlException on error
	//!
	static void Bind( sqlite3_stmt* handle, int index, const std::string& data )
	{
		int errorCode = sqlite3_bind_text(
			handle,
			index,
			data.c_str(),
			-1,
			SQLITE_TRANSIENT
			);

		utils::CheckErrorCode( errorCode, "SqliteOps<std::string>::Bind" );
	}

	//!
	//!	\brief	Gets a column from a statement
	//!
	//!	\param	statement	The statement to retreive from
	//!	\param	index		The index of this column ( 0 based )
	//!
	//!	\return	The data contained in the specified column
	//!
	//!	\throws A SqlException on error
	//!
	static std::string GetColumn( sqlite3_stmt* handle, int index )
	{
		return std::string( 
			reinterpret_cast<const char*>(
				sqlite3_column_text( handle, index )
				) );
	}
};

//TODO: These integer operations could generally be done better with boost enable_if etc.

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	SqliteOps specialization for unsigned ints
//!
///////////////////////////////////////////////////////////////////////////////
template<>
class SqliteOps< unsigned int >
{
public:
	//!
	//!	\brief	Binds data to a statement
	//!
	//!	\param	statement	The statement to bind to
	//!	\param	index		The index of this parameter ( 1 based )
	//!	\param	data		The data to bind
	//!
	//!	\throws A SqlException on error
	//!
	static void Bind( sqlite3_stmt* handle, int index, unsigned int data )
	{
		int errorCode = sqlite3_bind_int64(
			handle,
			index,
			data
			);

		utils::CheckErrorCode( errorCode, "SqliteOps< unsigned int >::Bind" );
	}

	//!
	//!	\brief	Gets a column from a statement
	//!
	//!	\param	statement	The statement to retreive from
	//!	\param	index		The index of this column ( 0 based )
	//!
	//!	\return	The data contained in the specified column
	//!
	//!	\throws A SqlException on error
	//!
	static unsigned int GetColumn( sqlite3_stmt* handle, int index )
	{
		return sqlite3_column_int( handle, index );
	}
};

//TODO: These integer operations could generally be done better with boost enable_if etc.

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	SqliteOps specialization for int
//!
///////////////////////////////////////////////////////////////////////////////
template<>
class SqliteOps< int >
{
public:
	//!
	//!	\brief	Binds data to a statement
	//!
	//!	\param	statement	The statement to bind to
	//!	\param	index		The index of this parameter ( 1 based )
	//!	\param	data		The data to bind
	//!
	//!	\throws A SqlException on error
	//!
	static void Bind( sqlite3_stmt* handle, int index, int data )
	{
		int errorCode = sqlite3_bind_int(
			handle,
			index,
			data
			);

		utils::CheckErrorCode( errorCode, "SqliteOps< unsigned int >::Bind" );
	}

	//!
	//!	\brief	Gets a column from a statement
	//!
	//!	\param	statement	The statement to retreive from
	//!	\param	index		The index of this column ( 0 based )
	//!
	//!	\return	The data contained in the specified column
	//!
	//!	\throws A SqlException on error
	//!
	static int GetColumn( sqlite3_stmt* handle, int index )
	{
		return sqlite3_column_int( handle, index );
	}
};

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	SqliteOps specialization for bool
//!
///////////////////////////////////////////////////////////////////////////////
template<>
class SqliteOps< bool >
{
public:
	//!
	//!	\brief	Binds data to a statement
	//!
	//!	\param	statement	The statement to bind to
	//!	\param	index		The index of this parameter ( 1 based )
	//!	\param	data		The data to bind
	//!
	//!	\throws A SqlException on error
	//!
	static void Bind( sqlite3_stmt* handle, int index, bool data )
	{
		int errorCode = sqlite3_bind_int(
			handle,
			index,
			data ? 1 : 0
			);

		utils::CheckErrorCode( errorCode, "SqliteOps< unsigned int >::Bind" );
	}

	//!
	//!	\brief	Gets a column from a statement
	//!
	//!	\param	statement	The statement to retreive from
	//!	\param	index		The index of this column ( 0 based )
	//!
	//!	\return	The data contained in the specified column
	//!
	//!	\throws A SqlException on error
	//!
	static bool GetColumn( sqlite3_stmt* handle, int index )
	{
		return sqlite3_column_int( handle, index ) != 0;
	}
};

}	// namespace db
}	// namespace kode

#endif	// KODE_DB_SQLITEOPS_H_
