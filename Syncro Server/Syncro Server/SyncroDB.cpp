#include "SyncroDB.h"

namespace syncro {

const int CSyncroDB::EXPECTED_DB_VERSION = 5;

const std::string FOLDERS_TABLE_NAME = "Folders";
const std::string FOLDERS_TABLE_CREATE = 
				"CREATE TABLE IF NOT EXISTS " + FOLDERS_TABLE_NAME + " (" +
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
                "Name TEXT NOT NULL, " +
                "Path TEXT NOT NULL, " + 
                "Writable INTEGER NOT NULL DEFAULT 0, " +
				"Readable INTEGER NOT NULL DEFAULT 1);";

const std::string SERVER_ID_TABLE_NAME = "ServerID";
const std::string SERVER_ID_TABLE_CREATE = 
				"CREATE TABLE IF NOT EXISTS " + SERVER_ID_TABLE_NAME + " (" + 
				"uuid TEXT NOT NULL," +
				"servername TEXT NOT NULL);";

const std::string USERS_TABLE_NAME = "Users";
const std::string USERS_TABLE_CREATE = 
				"CREATE TABLE IF NOT EXISTS " + USERS_TABLE_NAME + " (" +
				"ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
				"Username TEXT NOT NULL, " +
				"Password TEXT NOT NULL);";

//TODO: Move the database versioning stuff out into kode probably, then subclass that here.

CSyncroDB::CSyncroDB(std::string insFilename) : Database( insFilename ) {
	int nCurrentVersion = runScalar<int>("PRAGMA user_version;");
	
	if( nCurrentVersion != EXPECTED_DB_VERSION ) {
		bool fOK;
		if( nCurrentVersion == 0 ) {
			fOK = CreateDatabase();
		} else {
			fOK = UpgradeDatabase( nCurrentVersion );
		}
		if( fOK )
			run( "PRAGMA user_version=" + boost::lexical_cast<std::string>(EXPECTED_DB_VERSION) + ";");
	}
}

CSyncroDB::~CSyncroDB() {

}

bool CSyncroDB::CreateDatabase() {
	//TODO: possibly add a new function to database that doesn' return a result set
	run(FOLDERS_TABLE_CREATE);
	run(SERVER_ID_TABLE_CREATE);
	run(USERS_TABLE_CREATE);
	clearResult();
	return true;
}

bool CSyncroDB::UpgradeDatabase(int nCurrentVersion) {
	if( nCurrentVersion == 1 )
		run(SERVER_ID_TABLE_CREATE);
	if( nCurrentVersion == 2 ) {
		run("DROP TABLE " + SERVER_ID_TABLE_NAME + ";");
		run(SERVER_ID_TABLE_CREATE);
	}
	if( nCurrentVersion < 4 ) {
		run( USERS_TABLE_CREATE );
	}
	clearResult();
	return true;
}

};		//namespace syncro