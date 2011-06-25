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

#ifndef KODE_DB_RESULTSET_H_
#define KODE_DB_RESULTSET_H_

#include <kode/utils.h>
#include <vector>
#include <string>
#include <map>

namespace kode {
namespace db {

typedef std::map<
				std::string, 
				std::string, 
				kode::utils::CStringLessThan
		> Row;

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	A result set class for database operations
//!
//!			Note: This class was written ages ago, and is likely
//!					horribly inefficient.  Use of statements is likely
//!					better in every way.  It may be better to refactor
//!					this code heavily if it's ever needed
//!
///////////////////////////////////////////////////////////////////////////////
class ResultSet
{
	friend class Database;
protected:
	std::vector<Row> rows;
	std::vector<std::string> colNames;
public:
	ResultSet() 
	{ }

	ResultSet( const ResultSet& inoOther ) : 
	rows( inoOther.rows.begin(), inoOther.rows.end() ),
	colNames( inoOther.colNames.begin(), inoOther.colNames.end() ) 
	{}

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

	Row operator[]( int ID )
	{
		return rows[ID];
	};
	Row getRow( int ID )
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
	void add( Row row )
	{
		rows.push_back( row );
	};
	void defineCols( int num, char** names )
	{
		for( int i = 0; i < num; i++ )
			colNames.push_back( std::string( names[i] ) );
	}
	Row find( std::string col, std::string value )
	{
		BOOST_FOREACH( Row & oRow, rows )
		{
			Row::iterator pCol = oRow.find( col );
			if(( pCol != oRow.end() ) && ( pCol->second == value ) )
				return Row( oRow );
		}
		return Row();
	}
	//TODO: Fix logging at some point
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

}		// namespace kode 
}		// namespace db 

#endif	// KODE_DB_RESULTSET_H_