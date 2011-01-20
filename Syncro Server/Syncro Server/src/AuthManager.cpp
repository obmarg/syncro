#include "AuthManager.h"
#include "SyncroDB.h"
#include <kode/base64.h>
#include <cryptopp/hmac.h>
#include <boost/foreach.hpp>
#include <iostream>

namespace syncro
{

CAuthManager::CAuthManager()
{
	m_pDB = CSyncroDB::OpenDB();
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

CAuthManager::~CAuthManager()
{

}

bool CAuthManager::NeedsAuth()
{
	return false;
}

const CAuthToken CAuthManager::Authenticate( const std::string& username, const std::string& password, const std::string& ip )
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
		return CAuthToken( username, nID, CAuthToken::AccessLevel_Admin );
	}
	throw authentication_exception( "Invalid username or password" );
}

const CAuthToken CAuthManager::DefaultAuth()
{
	if( NeedsAuth() )
		throw authentication_exception( "Authentication required but no details provided" );
	return CAuthToken( "default", -1, CAuthToken::AccessLevel_Normal );
}

}
