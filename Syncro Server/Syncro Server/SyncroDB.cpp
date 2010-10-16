#include "SyncroDB.h"

namespace syncro {

const int CSyncroDB::EXPECTED_DB_VERSION = 1;

const std::string FOLDERS_TABLE_NAME = "Folders";
const std::string FOLDERS_TABLE_CREATE = 
				"CREATE TABLE IF NOT EXISTS " + FOLDERS_TABLE_NAME + " (" +
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
                "Name TEXT NOT NULL, " +
                "Path TEXT NOT NULL, " + 
                "Writable INTEGER NOT NULL DEFAULT 0, " +
				"Readable INTEGER NOT NULL DEFAULT 1);";

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
	clearResult();
	return true;
}

bool CSyncroDB::UpgradeDatabase(int nCurrentVersion) {
	return true;
}

};		//namespace syncro