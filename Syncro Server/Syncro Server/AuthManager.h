#ifndef _AUTH_MANAGER_H_
#define _AUTH_MANAGER_H_

#include "common.h"
#include <boost/shared_ptr.hpp>
#include "Database.h"

namespace syncro {

class CAuthToken {
public:
	enum AccessLevel {
		AccessLevel_Admin,
		AccessLevel_Normal
	};

	CAuthToken(const std::string sUsername, const int innID, const AccessLevel innAccessLevel) : 
		m_sUsername( sUsername ), 
		m_nUserID( innID ),
		m_eAccessLevel( innAccessLevel ) 
		{};

	const std::string GetUsername() const {
		return m_sUsername;
	}

	const int GetUserID() const {
		return m_nUserID;
	}
	
	const AccessLevel GetAccessLevel() const {
		return m_eAccessLevel;
	}

private:
	std::string m_sUsername;
	int m_nUserID;
	AccessLevel m_eAccessLevel;
};

class CAuthManager {
public:
	CAuthManager();
	~CAuthManager();

	bool NeedsAuth();
	const CAuthToken Authenticate(std::string username, std::string password);

private:
	db::DatabasePtr m_pDB;
	db::StatementPtr m_pCheckLoginStatement;
};

}
#endif