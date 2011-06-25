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

#include "kode/Database.h"
#include "kode/db/statement.h"
//#include "Logger.h"
#include <ctype.h>

namespace kode {
namespace db {

using std::string;

//extern CLogger Logger;
int callback( void*, int, char**, char** );

void stringToLower( sqlite3_context* db, int argc, sqlite3_value** argv );

Database::Database( const std::string& file )
{
#ifndef SQLITE_SINGLE_THREADED
	if( !sqlite3_threadsafe() )
		throw std::logic_error( "Sqlite not compiled threadsafe!" );
#endif

	int rc = sqlite3_open( file.c_str(), &db );
	if( rc )
	{
		sqlite3_close( db );
		std::string error = "Can't open database: " + file;
		throw std::runtime_error( error.c_str() );
	}
	sqlite3_create_function( db, "strToLower", 1, SQLITE_UTF8, 0, stringToLower, 0, 0 );
#ifdef USING_PTHREADS
	pthread_mutex_init( &mutex, 0 );
#endif
}

Database::~Database()
{
#ifdef USING_PTHREADS
	pthread_mutex_destroy( &mutex );
#endif
	sqlite3_close( db );
}

void Database::runInsert( std::string query )
{
	char* errorMsg = 0;

	int rc = sqlite3_exec( db, query.c_str(), NULL, NULL, &errorMsg );

	if( rc != SQLITE_OK )
	{
		std::stringstream ss;
		ss << "SQL Error";
		if( errorMsg != NULL )
		{
			ss << ": " << errorMsg;
		}
		sqlite3_free( errorMsg );
		throw SqlException( ss.str(), rc );
	}
}

ResultSet Database::run( std::string query )
{
#ifdef USING_PTHREADS
	pthread_mutex_lock( &mutex );
#endif
#if SQL_DEBUG
	Logger.Log( "Query: " + query, LOG_STDOUT );
#endif
	clearResult();
	char* errorMsg = 0;
	int rc = sqlite3_exec( db, query.c_str(), callback, this, &errorMsg );
	if( rc != SQLITE_OK )
	{
		std::stringstream ss;
		ss << "SQL Error";
		if( errorMsg != NULL )
		{
			ss << ": " << errorMsg;
		}
		sqlite3_free( errorMsg );
		throw SqlException( ss.str(), rc );
	}
#if USING_PTHREADS
	pthread_mutex_unlock( &mutex );
#endif
	return result;
}

void Database::clearResult()
{
	result.clear();
}

StatementPtr Database::prepare( std::string insSql )
{
	sqlite3_stmt* handle = NULL;
	StatementPtr rv;
	try
	{
		int nErrorCode = sqlite3_prepare_v2( this->db, insSql.c_str(), insSql.size(), &handle, NULL );
		if( nErrorCode != SQLITE_OK )
		{
			throw new SqlException( "Could not create statement: " + insSql, nErrorCode );
		}
		rv.reset( new Statement( handle ) );
	}
	catch( const std::exception& ex )
	{
		//TODO: do something
		if( handle != NULL && !rv )
			sqlite3_finalize( handle );
		throw ex;
	}
	return rv;
}

int callback( void* ptrDB, int numCols, char** colValues, char** colNames )
{
	Database* db = static_cast<Database*>( ptrDB );
	if( db->result.numCols() == 0 )
		db->result.defineCols( numCols, colNames );
	Row row;
	for( int i = 0; i < numCols; i++ )
	{
		row[colNames[i]] = colValues[i];
	}
	db->result.add( row );
	return 0;
}

void stringToLower( sqlite3_context* db, int argc, sqlite3_value** argv )
{
	//TODO: add error checking?
	char* text = ( char* )sqlite3_value_text( argv[0] );
	for( unsigned int i = 0; i < strlen( text ); i++ )
	{
		text[i] = tolower( text[i] );
	}
	sqlite3_result_text( db, text, strlen( text ), SQLITE_TRANSIENT );
	//TODO: find out if i need to delete the text here.
}

}	//namespace db
};	//namespace kode
