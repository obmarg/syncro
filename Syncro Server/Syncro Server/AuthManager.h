#ifndef _AUTH_MANAGER_H_
#define _AUTH_MANAGER_H_

#include "common.h"
#include <boost/shared_ptr.hpp>
#include <kode/Database.h>

namespace syncro {

class CAuthManager {
public:
	CAuthManager();
	~CAuthManager();

	bool NeedsAuth();
	const CAuthToken Authenticate(std::string username, std::string password);

private:
	kode::db::DatabasePtr m_pDB;
	kode::db::StatementPtr m_pCheckLoginStatement;
};

}
#endif