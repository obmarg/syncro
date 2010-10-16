#ifndef __DATABASE_H_
#define __DATABASE_H_

//#include "Logger.h"
//extern CLogger Logger;

#include <string>
#include <sqlite3.h>

#include <vector>
#include <map>

#include <time.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

#include "utils.h"

#ifdef USING_PTHREADS
#include <pthread.h>
#endif

namespace syncro {

class Database
{
public:
	typedef boost::shared_ptr<Database> TPointer;
	
	typedef std::map<std::string,std::string,CStringLessThan> Row;

	class ResultSet
	{
		friend class Database;
	protected:
		std::vector<Row> rows;
		std::vector<std::string> colNames;
	public:
		ResultSet() {};
		ResultSet(ResultSet& inoOther) : rows( inoOther.rows.begin(), inoOther.rows.end() ), colNames( inoOther.colNames.begin(), inoOther.colNames.end() ) { };

		typedef std::vector<Row>::iterator iterator;
		typedef std::vector<Row>::const_iterator const_iterator;
		iterator begin() { return rows.begin(); };
		iterator end() { return rows.end(); };
		const_iterator begin() const { return rows.begin(); };
		const_iterator end() const { return rows.end(); };

		bool empty() const { 
			if( rows.empty() )
				return true; 
			else 
				return colNames.empty();
		}

		Database::Row operator[](int ID){return rows[ID];};
		Database::Row getRow(int ID) {return rows[ID];};
		int numRows() {return rows.size();};
		int numCols() {return colNames.size();};
		void clear() { rows.clear(); colNames.clear(); };
		void add(Database::Row row) { rows.push_back(row); };
		void defineCols(int num,char **names)
		{
			for(int i =0;i<num;i++)
				colNames.push_back(std::string(names[i]));
		}
		Database::Row find(std::string col,std::string value)
		{
			BOOST_FOREACH( Row& oRow, rows ) {
				Row::iterator pCol = oRow.find( col );
				if( ( pCol != oRow.end() ) && ( pCol->second == value ) )
					return Row(oRow);
			}
			return Row();
		}
		void log(int level)
		{
			if(numRows() != 0)
			{
				std::string names = "Columns:";
				for(unsigned int i=0;i<colNames.size();i++)
					names += colNames[i] + "|";
//				Logger.Log(names,level);
				for(unsigned int i=0;i<rows.size();i++)
				{
					std::string values = "";
					for(unsigned int y=0;y<colNames.size();y++)
						values += rows[i][colNames[y]] + "|";
					//Logger.Log(values,level);
				}
			}
		}
	};
	
	friend int callback(void*,int,char**,char**);

	ResultSet run(std::string query);

	template<class tReturnType>
	tReturnType runScalar(std::string query) {
		ResultSet oResults = run(query);
		if( oResults.empty() )
			throw std::out_of_range("DB::runScalar call returned nothing");
		const std::string& sColName = oResults.colNames[0];
		const Row& oRow = oResults[0];
		Row::const_iterator oData = oRow.find( sColName );
		if( oData != oRow.end() ) {
			return boost::lexical_cast<tReturnType,std::string>( oData->second );
		} else {
			throw std::out_of_range("DB::runScalar call returned nothing");
		}
		result.clear();
	}

	template<>
	std::string runScalar<std::string>(std::string query) {
		ResultSet oResults = run(query);
		if( oResults.empty() ) {
			throw std::out_of_range("DB::runScalar call returned nothing");
		}
		const std::string& sColName = oResults.colNames[0];
		const Row& oRow = oResults[0];
		Row::const_iterator oData = oRow.find( sColName );
		if( oData != oRow.end() ) {
			return oData->second;
		} else {
			throw std::out_of_range("DB::runScalar call returned nothing");
		}
		result.clear();
	}

private:
	sqlite3 *db;
	ResultSet result;

#ifdef USING_PTHREADS
	pthread_mutex_t mutex;
#endif

protected:
	void clearResult();

	Database(std::string file);
	virtual ~Database();
};

}; //namespace syncro

#endif
