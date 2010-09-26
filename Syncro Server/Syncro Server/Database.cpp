#include "Database.h"
//#include "Logger.h"
#include <ctype.h>

//extern CLogger Logger;
//static int callback(void*,int,char**,char**);

void stringToLower(sqlite3_context* db,int argc,sqlite3_value** argv);

Database::Database(string file)
{
	int rc = sqlite3_open(file.c_str(),&db);
	if( rc )
	{
		//fail
		//TODO: Exception?
		sqlite3_close(db);	
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

Database::ResultSet Database::run(string query)
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
		//TODO: exception
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
