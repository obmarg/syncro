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

#ifndef KODE_DB_STATEMENT_H_
#define KODE_DB_STATEMENT_H_

#include "kode/db/sqliteexception.h"
#include "kode/db/sqliteops.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

struct sqlite3_stmt;

namespace kode {
namespace db {

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	A class representing a sqlite statement.
//!
///////////////////////////////////////////////////////////////////////////////
class Statement
{
	//TODO: Make statement support pthreads stuff?
	friend class Database;
public:
	virtual ~Statement();
	
	bool GetNextRow();
	
	void Reset();

	template<class tData>
	Statement& Bind( int index, tData data );

	template<class tData>
	Statement& Bind( std::string parameter, tData data );

	template<class tData>
	tData GetColumn( int index );

	template<class tData>
	tData GetColumn( std::string parameter );

protected:
	Statement( sqlite3_stmt* handle );

	void FetchColumnNames();

	int FindColumn( std::string colName );	

private:
	sqlite3_stmt* 							m_handle;
	std::vector<std::string> 				m_columnNames;
	bool 									m_fFetchedNames;
};

typedef boost::shared_ptr<Statement> StatementPtr;

template<class tData>
Statement& Statement::Bind( int index, tData data )
{
	SqliteOps<tData>::Bind( m_handle, index, data );
	return ( *this );
}

template<class tData>
Statement& Statement::Bind( std::string parameter, tData data )
{
	int nIndex = sqlite3_bind_parameter_index( m_handle, parameter.c_str() );
	Bind( nIndex, data );
	return ( *this );
}

template<class tData>
tData Statement::GetColumn( int index )
{
	return SqliteOps<tData>::GetColumn( m_handle, index );
}

template<class tData>
tData Statement::GetColumn( std::string parameter )
{
	if( !m_fFetchedNames )
		FetchColumnNames();

	int nIndex = FindColumn( parameter );
	if( nIndex == -1 )
		throw SqlException( "Invalid column passed to Statement::GetColumn: " + parameter, 0 );
	return GetColumn<tData>( nIndex );
}

///////////////////////////////////////////////////////////////////////////////
//
//	\brief	A class to automatically reset a statement on destruction
//
///////////////////////////////////////////////////////////////////////////////
class AutoReset : boost::noncopyable
{
public:
	AutoReset( const StatementPtr statement ) : m_statement( statement ) {};
	~AutoReset()
	{
		m_statement->Reset();
	};
private:
	const StatementPtr m_statement;
};

}	// namespace db
} 	// namespace kode

#endif	// KODE_DB_STATEMENT_H_
