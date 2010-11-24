#ifndef _AUTH_MANAGER_H_
#define _AUTH_MANAGER_H_

#include "common.h"
#include <kode/Database.h>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace syncro {

class CAuthManager {
public:
	CAuthManager();
	~CAuthManager();

	bool NeedsAuth();
	const CAuthToken Authenticate(const std::string& username, const std::string& password, const std::string& ip);

private:
	kode::db::DatabasePtr m_pDB;
	kode::db::StatementPtr m_pCheckLoginStatement;
};

}
#endif