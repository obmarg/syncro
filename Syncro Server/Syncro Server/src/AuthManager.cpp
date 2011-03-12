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

#include "AuthManager.h"
#include "SyncroDB.h"
#include <kode/base64.h>
#include <cryptopp/hmac.h>
#include <boost/foreach.hpp>
#include <iostream>

namespace syncro
{

AuthManager::AuthManager()
{
	m_pDB = SyncroDB::OpenDB();
	m_pCheckLoginStatement = m_pDB->prepare( "SELECT ID,Password FROM Users WHERE UserName=@Username" );
	std::string uuid = kode::utils::GenerateUUID();

	CryptoPP::SHA().CalculateDigest(
	    m_salt,
	    reinterpret_cast< const unsigned char* >( uuid.c_str() ),
	    uuid.length()
	);

	m_saltString = kode::base64::Encode( m_salt, sizeof( m_salt ) );
	std::cout << "Salt: " << m_saltString << "\n";
}

AuthManager::~AuthManager()
{

}

bool AuthManager::NeedsAuth()
{
	return false;
}

const AuthToken AuthManager::Authenticate( const std::string& username, const std::string& password, const std::string& ip )
{
	kode::db::AutoReset autoReset( m_pCheckLoginStatement );

	m_pCheckLoginStatement->Bind( "@Username", username );
	if( m_pCheckLoginStatement->GetNextRow() )
	{

		int nID = m_pCheckLoginStatement->GetColumn<int>( 0 );
		std::string encodedHash = m_pCheckLoginStatement->GetColumn<std::string>( 1 );
		std::vector< unsigned char > passwordHash( CryptoPP::SHA::DIGESTSIZE );
		kode::base64::Decode( encodedHash, passwordHash );

		//
		// Now we need to confirm the passwords match
		//
		unsigned char hashBuffer[ CryptoPP::SHA::DIGESTSIZE ];
		CryptoPP::HMAC< CryptoPP::SHA >( m_salt, sizeof( m_salt ) )
		.CalculateDigest(
		    hashBuffer,
		    &passwordHash[0],
		    passwordHash.size()
		);
		std::string hmac = kode::base64::Encode( hashBuffer, sizeof( hashBuffer ) );

		if( hmac != password )
			throw authentication_exception( "Invalid username or password" );
		return AuthToken( username, nID, AuthToken::AccessLevel_Admin );
	}
	throw authentication_exception( "Invalid username or password" );
}

const AuthToken AuthManager::DefaultAuth()
{
	if( NeedsAuth() )
		throw authentication_exception( "Authentication required but no details provided" );
	return AuthToken( "default", -1, AuthToken::AccessLevel_Normal );
}

}
