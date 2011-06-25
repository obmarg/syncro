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
#include "kode/db/resultset.h"
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

	friend int callback( void*, int, char**, char** );

	void runInsert( std::string query );
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

}	// namespace db
} 	// namespace kode

#endif
