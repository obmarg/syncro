/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _AUTH_MANAGER_H_
#define _AUTH_MANAGER_H_

#include "common.h"
#include <kode/Database.h>
#include <cryptopp/sha.h>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace syncro
{

class CAuthManager
{
public:
	CAuthManager();
	~CAuthManager();

	bool NeedsAuth();
	const CAuthToken Authenticate( const std::string& username, const std::string& password, const std::string& ip );
	const std::string Salt() const
	{
		return m_saltString;
	};

	const CAuthToken DefaultAuth();

private:
	kode::db::DatabasePtr m_pDB;
	kode::db::StatementPtr m_pCheckLoginStatement;
	std::string m_saltString;
	unsigned char m_salt[ CryptoPP::SHA::DIGESTSIZE ];
};

}
#endif
