#include "Database.h"
//#include "Logger.h"
#include <ctype.h>

namespace kode {
namespace db {

using std::string;

//extern CLogger Logger;
int callback(void*,int,char**,char**);

void stringToLower(sqlite3_context* db,int argc,sqlite3_value** argv);

Database::Database(std::string file)
{
	int rc = sqlite3_open(file.c_str(),&db);
	if( rc )
	{
		sqlite3_close(db);
		std::string error = "Can't open database: " + file;
		throw std::runtime_error( error.c_str() );
	}
	sqlite3_create_function(db,"strToLower",1,SQLITE_UTF8,0,stringToLower,0,0);
#ifdef USING_PTHREADS
	pthread_mutex_init(&mutex,0);
#endif
}

Database::~Database()
{
#ifdef USING_PTHREADS
	pthread_mutex_destroy(&mutex);
#endif
	sqlite3_close(db);
}

Database::ResultSet Database::run(std::string query)
{
#ifdef USING_PTHREADS
	pthread_mutex_lock(&mutex);
#endif
#if SQL_DEBUG
	Logger.Log("Query: "+query,LOG_STDOUT);
#endif
	clearResult();
	char *errorMsg = 0;
	int rc = sqlite3_exec(db, query.c_str(),callback,this,0);
	//int rc = sqlite3_exec(db, query.c_str(),callback,this,&errorMsg);
	if(rc != SQLITE_OK)
	{
		//Logger.Log("Sqlite error.  Query not executed",LOG_ERROR);
		//error
		std::string error = "SQL Error: ";
		if( errorMsg != NULL ) {
			error += std::string( errorMsg );
		}
		error += "\nRunning query: " + query;
		throw std::runtime_error( error.c_str() );
		sqlite3_free(errorMsg);
	}
#if USING_PTHREADS
	pthread_mutex_unlock(&mutex);
#endif
	return result;
}

void Database::clearResult()
{
	result.clear();
}

StatementPtr Database::prepare(std::string insSql)
{
	sqlite3_stmt *handle = NULL;
	StatementPtr rv;
	try {
		int nErrorCode = sqlite3_prepare_v2( this->db, insSql.c_str(), insSql.size(), &handle, NULL );
		if( nErrorCode != SQLITE_OK ) {
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

int callback(void* ptrDB,int numCols, char **colValues,char** colNames)
{
	Database *db = static_cast<Database*>(ptrDB);
	if(db->result.numCols() == 0)
		db->result.defineCols(numCols,colNames);
	Database::Row row;
	for(int i =0;i<numCols;i++)
	{
		row[colNames[i]] = colValues[i];
	}
	db->result.add(row);
	return 0;
}

void stringToLower(sqlite3_context* db,int argc,sqlite3_value** argv)
{
	//TODO: add error checking?
	char* text = (char*)sqlite3_value_text(argv[0]);
	for(unsigned int i =0;i<strlen(text);i++)
	{
		text[i] = tolower(text[i]);
	}
	sqlite3_result_text(db,text,strlen(text),SQLITE_TRANSIENT);
	//TODO: find out if i need to delete the text here.
}

}	//namespace db
};	//namespace kode
