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

#include "kode/db/statement.h"
//#include "sqlite3.h"
#include <boost/foreach.hpp>

namespace kode {
namespace db {

Statement::Statement( sqlite3_stmt* handle ) : 
m_handle( handle ), 
m_fFetchedNames( false ) 
{
}

Statement::~Statement()
{
	sqlite3_finalize( m_handle );
}

bool Statement::GetNextRow()
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

void Statement::Reset()
{
	sqlite3_reset( m_handle );
}

void Statement::FetchColumnNames()
{
	int nCount = sqlite3_column_count( m_handle );
	m_columnNames.reserve( nCount );
	for( int iNum = 0; iNum < nCount; ++iNum )
	{
		m_columnNames.push_back( std::string( sqlite3_column_name( m_handle, iNum ) ) );
	}
	m_fFetchedNames = true;
}

int Statement::FindColumn( std::string colName )
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

}	// namespace db
}	// namespace kode


