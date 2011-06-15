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

#ifndef __DATABASE_H_
#define __DATABASE_H_

//#include "Logger.h"
//extern CLogger Logger;

#include "kode/db/sqliteexception.h"
#include "kode/db/sqliteops.h"
#include "kode/utils.h"
#include <sqlite3.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <time.h>

#ifdef USING_PTHREADS
#include <pthread.h>
#endif

namespace kode {
namespace db {

class Database;
class Statement;
typedef boost::shared_ptr<Database> DatabasePtr;
typedef boost::shared_ptr<Statement> StatementPtr;

class Database
{
public:
	typedef boost::shared_ptr<Database> TPointer;

	typedef std::map<std::string, std::string, kode::utils::CStringLessThan> Row;

	class ResultSet
	{
		friend class Database;
	protected:
		std::vector<Row> rows;
		std::vector<std::string> colNames;
	public:
		ResultSet() {};
		ResultSet( const ResultSet& inoOther ) : rows( inoOther.rows.begin(), inoOther.rows.end() ), colNames( inoOther.colNames.begin(), inoOther.colNames.end() ) { };

		/*ResultSet& operator=(const syncro::Database::ResultsSet& inoRHS) {
			rows = inoRHS.rows;
			colNames = inoRHS.colNames;
		}*/

		typedef std::vector<Row>::iterator iterator;
		typedef std::vector<Row>::const_iterator const_iterator;
		iterator begin()
		{
			return rows.begin();
		};
		iterator end()
		{
			return rows.end();
		};
		const_iterator begin() const
		{
			return rows.begin();
		};
		const_iterator end() const
		{
			return rows.end();
		};

		bool empty() const
		{
			if( rows.empty() )
				return true;
			else
				return colNames.empty();
		}

		Database::Row operator[]( int ID )
		{
			return rows[ID];
		};
		Database::Row getRow( int ID )
		{
			return rows[ID];
		};
		int numRows()
		{
			return rows.size();
		};
		int numCols()
		{
			return colNames.size();
		};
		void clear()
		{
			rows.clear();
			colNames.clear();
		};
		void add( Database::Row row )
		{
			rows.push_back( row );
		};
		void defineCols( int num, char** names )
		{
			for( int i = 0; i < num; i++ )
				colNames.push_back( std::string( names[i] ) );
		}
		Database::Row find( std::string col, std::string value )
		{
			BOOST_FOREACH( Row & oRow, rows )
			{
				Row::iterator pCol = oRow.find( col );
				if(( pCol != oRow.end() ) && ( pCol->second == value ) )
					return Row( oRow );
			}
			return Row();
		}
		void log( int level )
		{
			if( numRows() != 0 )
			{
				std::string names = "Columns:";
				for( unsigned int i = 0; i < colNames.size(); i++ )
					names += colNames[i] + "|";
//				Logger.Log(names,level);
				for( unsigned int i = 0; i < rows.size(); i++ )
				{
					std::string values = "";
					for( unsigned int y = 0; y < colNames.size(); y++ )
						values += rows[i][colNames[y]] + "|";
					//Logger.Log(values,level);
				}
			}
		}
	};

	friend int callback( void*, int, char**, char** );

	ResultSet run( std::string query );

	template<class tReturnType>
	tReturnType runScalar( std::string query )
	{
		ResultSet oResults = run( query );
		if( oResults.empty() )
			throw std::out_of_range( "DB::runScalar call returned nothing" );
		const std::string& sColName = oResults.colNames[0];
		const Row& oRow = oResults[0];
		Row::const_iterator oData = oRow.find( sColName );
		if( oData != oRow.end() )
		{
			tReturnType rv = boost::lexical_cast<tReturnType, std::string>( oData->second );
			result.clear();
			return rv;
		}
		else
		{
			result.clear();
			throw std::out_of_range( "DB::runScalar call returned nothing" );
		}
		//Control should never reach this point
		return tReturnType();
	}

	StatementPtr prepare( std::string insSql );

#ifdef _WIN32
//TODO: Figure out if this is just illegal syntax, or if it's gcc being stupid
	template<>
	std::string runScalar<std::string>( std::string query );
#endif

private:
	sqlite3* db;
	ResultSet result;

#ifdef USING_PTHREADS
	pthread_mutex_t mutex;
#endif

protected:
	void clearResult();

	Database( const std::string& file );
	virtual ~Database();
};

#ifdef _WIN32
//TODO: figure out how the fuck you do specialization like this in gcc
template<>
std::string Database::runScalar<std::string>( std::string query )
{
	ResultSet oResults( run( query ) );
	if( oResults.empty() )
	{
		throw std::out_of_range( "DB::runScalar call returned nothing" );
	}
	const std::string& sColName = oResults.colNames[0];
	const Row& oRow = oResults[0];
	Row::const_iterator oData = oRow.find( sColName );
	if( oData != oRow.end() )
	{
		std::string sRV = oData->second;
		result.clear();
		return sRV;
	}
	else
	{
		result.clear();
		throw std::out_of_range( "DB::runScalar call returned nothing" );
	}
	//Control should never reach this point
	return "";
}
#endif

class Statement
{
	//TODO: Make statement support pthreads stuff?
	friend class Database;
protected:
	Statement( sqlite3_stmt* handle ) : m_handle( handle ), m_fFetchedNames( false ) {};

	void FetchColumnNames()
	{
		int nCount = sqlite3_column_count( m_handle );
		m_columnNames.reserve( nCount );
		for( int iNum = 0; iNum < nCount; ++iNum )
		{
			m_columnNames.push_back( std::string( sqlite3_column_name( m_handle, iNum ) ) );
		}
		m_fFetchedNames = true;
	}

	int FindColumn( std::string colName )
	{
		int nIndex = 0;
		BOOST_FOREACH( const std::string & current, m_columnNames )
		{
			if( current.compare( colName ) == 0 )
				return nIndex;
			++nIndex;
		}
		return -1;
	}

public:
	//TODO: THis could all be done better with template meta programming type stuff
	virtual ~Statement()
	{
		sqlite3_finalize( m_handle );
	}

	bool GetNextRow()
	{
		int nErrorCode = sqlite3_step( m_handle );
		switch( nErrorCode )
		{
		case SQLITE_BUSY:
			throw SqlException( "Could not get next row - database busy", nErrorCode );
		case SQLITE_ROW:
			return true;
		case SQLITE_DONE:
			return false;
		default:
			throw SqlException( "Error in Statement::GetNextRow", nErrorCode );
		}
	}

	void Reset()
	{
		sqlite3_reset( m_handle );
	}

	template<class tData>
	Statement& Bind( int index, tData data );

	template<class tData>
	Statement& Bind( std::string parameter, tData data );

//#ifdef _WIN32
//TODO: FIgure out if moving shit from here works...
//#endif

	template<class tData>
	tData GetColumn( int index );

	template<class tData>
	tData GetColumn( std::string parameter );

//#ifdef _WIN32
	//TODO: Figre out if I can just remove these statements from here...
//#endif

private:
	sqlite3_stmt* 							m_handle;
	std::vector<std::string> 				m_columnNames;
	bool 									m_fFetchedNames;
};

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

#endif
