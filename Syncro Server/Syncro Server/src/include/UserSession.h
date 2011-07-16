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
#include "FolderMan.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace syncro {

class FileSendData;
class BinaryIncomingData;
class FolderMan;
class AdminCommandManager;

namespace server {

class UserSession
{
public:
	typedef boost::shared_ptr<FileSendData>		SendDataPtr;
	typedef boost::shared_ptr<BinaryIncomingData>	RecvDataPtr;
	typedef boost::shared_ptr<AdminCommandManager> AdminCommandManPtr;

	UserSession();
	
	SendDataPtr& GetCurrentSendData()
	{ return m_currentSendData; }

	RecvDataPtr& GetCurrentRecvData()
	{ return m_currentRecvData; }

	const AuthToken& GetAuthToken() const
	{ return m_authToken; }

	AdminCommandManPtr& GetAdminCommandMan()
	{ return m_adminCommandMan; }

	const std::string& GetSalt() const;

	FolderMan& GetFolderMan()
	{ return (*m_folderMan); }

	void Authenticate( 
		const std::string& username, 
		const std::string& password
		);
	void DefaultAuth();

private:
	SendDataPtr						m_currentSendData;
	RecvDataPtr						m_currentRecvData;

	AdminCommandManPtr				m_adminCommandMan;

	DatabasePtr						m_db;
	boost::scoped_ptr< FolderMan >	m_folderMan;

	bool							m_authenticated;
	AuthToken						m_authToken;
	AuthManager						m_authMan;
};

}	// namespace server
}	// namespace syncro

#endif		// SYNCRO_USER_SESSION_H_