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

#ifndef SYNCRO_USER_SESSION_H_
#define SYNCRO_USER_SESSION_H_

#include "common.h"
#include "AuthManager.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace syncro {

class CFileSendData;
class CBinaryIncomingData;
class CFolderMan;
class CAdminCommandManager;

namespace server {

class UserSession
{
public:
	typedef boost::scoped_ptr<CFileSendData>		SendDataPtr;
	typedef boost::scoped_ptr<CBinaryIncomingData>	RecvDataPtr;

	UserSession();
	
	SendDataPtr& GetCurrentSendData()
	{ return m_currentSendData; }

	RecvDataPtr& GetCurrentRecvData()
	{ return m_currentRecvData; }

	const CAuthToken& GetAuthToken() const
	{ return m_authToken; }

	const std::string& GetSalt() const;

	const CFolderMan& GetFolderMan()
	{ return (*m_folderMan); }

	void Authenticate( 
		const std::string& username, 
		const std::string& password
		);
	void DefaultAuth();

private:
	SendDataPtr m_currentSendData;
	RecvDataPtr m_currentRecvData;

	boost::shared_ptr< kode::db::Database >	m_db;
	boost::scoped_ptr<CFolderMan>			m_folderMan;

	bool					m_authenticated;
	CAuthToken				m_authToken;
	CAuthManager			m_authMan;
};

}	// namespace server
}	// namespace syncro

#endif		// SYNCRO_USER_SESSION_H_