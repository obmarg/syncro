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

#include <stdexcept>

#include "utils.h"

#ifdef USING_PTHREADS
#include <pthread.h>
#endif

namespace kode {
namespace db {

class SqlException : std::runtime_error {
public:
	SqlException( std::string sMessage, int nErrorCode ) : runtime_error( sMessage.c_str() ) {
		m_nErrorCode = nErrorCode;
	}
private:
	int m_nErrorCode;
};

class Database;
class Statement;
typedef boost::shared_ptr<Database> DatabasePtr;
typedef boost::shared_ptr<Statement> StatementPtr;


class Database
{
public:
	typedef boost::shared_ptr<Database> TPointer;	

	typedef std::map<std::string,std::string,utils::CStringLessThan> Row;

	class ResultSet
	{
		friend class Database;
	protected:
		std::vector<Row> rows;
		std::vector<std::string> colNames;
	public:
		ResultSet() {};
		ResultSet(const ResultSet& inoOther) : rows( inoOther.rows.begin(), inoOther.rows.end() ), colNames( inoOther.colNames.begin(), inoOther.colNames.end() ) { };
		
		/*ResultSet& operator=(const syncro::Database::ResultsSet& inoRHS) {
			rows = inoRHS.rows;
			colNames = inoRHS.colNames;
		}*/

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
			tReturnType rv = boost::lexical_cast<tReturnType,std::string>( oData->second );
			result.clear();
			return rv;
		} else {
			result.clear();
			throw std::out_of_range("DB::runScalar call returned nothing");
		}
		//Control should never reach this point
		return tReturnType();
	}

	StatementPtr prepare(std::string insSql);

#ifdef _WIN32
	template<>
	std::string runScalar<std::string>(std::string query);
#endif

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

#ifdef _WIN32
//TODO: figure out how the fuck you do specialization like this in gcc
template<>
std::string Database::runScalar<std::string>(std::string query) {
	ResultSet oResults( run(query) );
	if( oResults.empty() ) {
		throw std::out_of_range("DB::runScalar call returned nothing");
	}
	const std::string& sColName = oResults.colNames[0];
	const Row& oRow = oResults[0];
	Row::const_iterator oData = oRow.find( sColName );
	if( oData != oRow.end() ) {
		std::string sRV = oData->second;
		result.clear();
		return sRV;
	} else {
		result.clear();
		throw std::out_of_range("DB::runScalar call returned nothing");
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
	Statement(sqlite3_stmt* handle) : m_handle(handle), m_fFetchedNames(false) {};

	void FetchColumnNames() {
		int nCount = sqlite3_column_count( m_handle );
		m_columnNames.reserve( nCount );
		for( int iNum = 0; iNum < nCount; ++iNum ) {
			m_columnNames.push_back( std::string( sqlite3_column_name( m_handle, iNum ) ) );
		}
		m_fFetchedNames = true;
	}

	int FindColumn(std::string colName) {
		int nIndex = 0;
		BOOST_FOREACH( const std::string& current, m_columnNames ) {
			if( current.compare( colName ) == 0 )
				return nIndex;
			++nIndex;
		}
		return -1;
	}
	
public:
	//TODO: THis could all be done better with template meta programming type stuff
	virtual ~Statement() {
		sqlite3_finalize( m_handle );
	}

	bool GetNextRow() {
		int nErrorCode = sqlite3_step( m_handle );
		switch( nErrorCode ) {
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

	void Reset() {
		sqlite3_reset( m_handle );
	}

	template<class tData>
	Statement& Bind(int innIndex, tData data) {
		int nErrorCode = sqlite3_bind_text( m_handle, innIndex, boost::lexical_cast<std::string>(data).c_str(), -1, SQLITE_TRANSIENT );
		if( nErrorCode != SQLITE_OK )
			throw SqlException( "Statement::Bind failed", nErrorCode );
		return (*this);
	}

	template<class tData>
	Statement& Bind(std::string parameter, tData data) {
		int nIndex = sqlite3_bind_parameter_index( m_handle, parameter.c_str() );
		Bind( nIndex, data );
		return (*this);
	}
#ifdef _WIN32
	template<>
	Statement& Bind<std::string>(int innIndex, std::string data) {
		int nErrorCode = sqlite3_bind_text( m_handle, innIndex, data.c_str(), -1, SQLITE_TRANSIENT );
		if( nErrorCode != SQLITE_OK )
			throw SqlException( "Statement::Bind failed", nErrorCode );
		return (*this);
	}
	
	template<>
	Statement& Bind<int>(int innIndex, int innData) {
		int nErrorCode = sqlite3_bind_int( m_handle, innIndex, innData );
		if( nErrorCode != SQLITE_OK )
			throw SqlException( "Statement::Bind failed", nErrorCode );
		return (*this);
	}

	template<>
	Statement& Bind<bool>(int innIndex, bool infData) {
		int nErrorCode = sqlite3_bind_int( m_handle, innIndex, ( infData ? 1 : 0 ) );
		if( nErrorCode != SQLITE_OK )
			throw SqlException( "Statement::Bind failed", nErrorCode );
		return (*this);
	}
	
	template<>
	Statement& Bind<unsigned int>(int innIndex, unsigned int innData) {
		int nErrorCode = sqlite3_bind_int64( m_handle, innIndex, innData );
		if( nErrorCode != SQLITE_OK )
			throw SqlException( "Statement::Bind failed", nErrorCode );
		return (*this);
	}

#endif

	template<class tData>
	tData GetColumn(int innIndex) {
		std::string sData = std::string( sqlite3_column_text( m_handle, innIndex ) );
		return boost::lexical_cast<tData,std::string>( sData );
	}

	template<class tData>
	tData GetColumn(std::string parameter) {
		if( !m_fFetchedNames )
			FetchColumnNames();

		int nIndex = FindColumn( parameter );
		if( nIndex == -1 )
			throw SqlException( "Invalid column passed to Statement::GetColumn: " + parameter, 0 );
		return GetColumn( nIndex, data );
	}

#ifdef _WIN32
	template<>
	std::string GetColumn<std::string>( int innIndex) {
		return std::string( reinterpret_cast<const char*>( sqlite3_column_text( m_handle, innIndex ) ) );
	}

	template<>
	int GetColumn<int>(int innIndex) {
		return sqlite3_column_int( m_handle, innIndex );
	}
	template<>
	bool GetColumn<bool>( int innIndex ) {
		return (sqlite3_column_int( m_handle, innIndex ) == 1);
	}
	template<>
	unsigned int GetColumn<unsigned int>( int innIndex ) {
		return static_cast<unsigned int>( sqlite3_column_int64( m_handle, innIndex ) );
	}
#endif

private:
	sqlite3_stmt* m_handle;
	std::vector<std::string> m_columnNames;
	bool m_fFetchedNames;
};

class AutoReset : boost::noncopyable {
public:
	AutoReset( const StatementPtr statement ) : m_statement( statement ) {};
	~AutoReset() {
		m_statement->Reset();
	};
private:
	const StatementPtr m_statement;
};

}; //namespace db
}; //namespace kode

#endif
