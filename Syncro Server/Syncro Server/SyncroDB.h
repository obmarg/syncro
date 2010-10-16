#ifndef __SYNCRO_DB_H_
#define __SYNCRO_DB_H_

#include "Database.h"

namespace syncro {

class CSyncroDB : public Database, boost::noncopyable {
public:
	static Database::TPointer OpenDB(std::string insFilename) {
		Database::TPointer pDatabase( new CSyncroDB(insFilename) );
		return pDatabase;
	}

	virtual ~CSyncroDB();
private:
	static const int EXPECTED_DB_VERSION;

	CSyncroDB(std::string insFile);

	bool CreateDatabase();
	bool UpgradeDatabase(int nCurrentVersion);
};

};		//namespace syncro

#endif