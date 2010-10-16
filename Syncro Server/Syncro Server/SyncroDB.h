#ifndef __SYNCRO_DB_H_
#define __SYNCRO_DB_H_

#include "Database.h"

namespace syncro {

class CSyncroDB : public Database {
private:
	static const int EXPECTED_DB_VERSION;

	void CreateDatabase();
	void UpgradeDatabase(int nCurrentVersion);
public:
	CSyncroDB(std::string insFile);
	~CSyncroDB();
};

};		//namespace syncro

#endif