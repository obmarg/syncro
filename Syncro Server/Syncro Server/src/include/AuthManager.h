#ifndef _AUTH_MANAGER_H_
#define _AUTH_MANAGER_H_

#include "common.h"
#include <kode/Database.h>
#include <cryptopp/sha.h>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace syncro {

class CAuthManager {
public:
	CAuthManager();
	~CAuthManager();

	bool NeedsAuth();
	const CAuthToken Authenticate(const std::string& username, const std::string& password, const std::string& ip);
	const std::string Salt() const { return m_saltString; };

	const CAuthToken DefaultAuth();

private:
	kode::db::DatabasePtr m_pDB;
	kode::db::StatementPtr m_pCheckLoginStatement;
	std::string m_saltString;
	unsigned char m_salt[ CryptoPP::SHA::DIGESTSIZE ];
};

}
#endif
