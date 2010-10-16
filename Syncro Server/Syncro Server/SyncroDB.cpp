#include "SyncroDB.h"

namespace syncro {

const int CSyncroDB::EXPECTED_DB_VERSION = 1;

const std::string FOLDERS_TABLE_NAME = "Folders";
const std::string FOLDERS_TABLE_CREATE = 
				"CREATE TABLE IF NOT EXISTS " + FOLDERS_TABLE_NAME + " (" +
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
                "Name TEXT, " +
                "Path TEXT, " + 
                "Writable INTEGER, " +
				"Readable INTEGER, ";

CSyncroDB::CSyncroDB(std::string insFilename) : Database( insFilename ) {
	int nCurrentVersion = runScalar<int>("PRAGMA user_version;");
	if( nCurrentVersion == 0 ) {
		CreateDatabase();
	} else if( nCurrentVersion < EXPECTED_DB_VERSION ) {
		UpgradeDatabase( nCurrentVersion );
	}
}

CSyncroDB::~CSyncroDB() {

}

void CSyncroDB::CreateDatabase() {
	//TODO: possibly add a new function to database that doesn' return a result set
	run(FOLDERS_TABLE_CREATE);
	clearResult();
}

void CSyncroDB::UpgradeDatabase(int nCurrentVersion) {

}

};		//namespace syncro