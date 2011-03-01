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

#include "UserSession.h"
#include "SyncroDB.h"
#include "FolderMan.h"
#include "AuthManager.h"
#include "FileSendData.h"
#include "BinaryIncomingData.h"

namespace syncro {
namespace server {

UserSession::UserSession() :
m_db( CSyncroDB::OpenDB( ) ),
m_folderMan( new CFolderMan( m_db ) )
{

}

void UserSession::Authenticate( 
	const std::string& username, 
	const std::string& password
	)
{
	m_authToken = m_authMan.Authenticate( username, password, "" );
	m_authenticated = true;
}

void UserSession::DefaultAuth()
{
	m_authToken = m_authMan.DefaultAuth();
}

const std::string& UserSession::GetSalt() const
{
	return m_authMan.Salt();
}

}
}	// namespace syncro