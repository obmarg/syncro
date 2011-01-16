#include "AuthManager.h"
#include "SyncroDB.h"
#include <kode/base64.h>
#include <cryptopp/hmac.h>
#include <iostream>

namespace syncro {

CAuthManager::CAuthManager() {
	m_pDB = CSyncroDB::OpenDB();
	m_pCheckLoginStatement = m_pDB->prepare("SELECT ID,Password FROM Users WHERE UserName=@Username");
	std::string uuid = kode::utils::GenerateUUID();
	
	CryptoPP::SHA().CalculateDigest( 
		m_salt, 
		reinterpret_cast< const unsigned char* >( uuid.c_str() ), 
		uuid.length() 
		);
#if 0
	CryptoPP::Base64Encoder encoder;
	encoder.PutMessageEnd( 
		m_salt,
	       	sizeof( m_salt ) 
		);
	if( !encoder.AnyRetrievable() )
		throw std::runtime_error( "Base64 error in CAuthManager::CAuthManager()" );

	unsigned int destSize = encoder.MaxRetrievable();
	std::vector< char > saltVector( destSize );

	unsigned int sizeRead = encoder.Get( 
		reinterpret_cast< unsigned char* >( &saltVector[0] ),
		destSize
		);
	if( sizeRead != destSize )
	{
		std::cout << "Warning: Salt vector read legnth is less than expected in " <<
			"CAuthManager::CAuthManager()\n";
		saltVector.resize( sizeRead );
	}

	m_saltString = std::string( saltVector.begin(), saltVector.end() );
#endif
	m_saltString = kode::base64::Encode( m_salt, sizeof( m_salt ) );
	std::cout << "Salt: " << m_saltString << "\n";
}

CAuthManager::~CAuthManager() {

}

bool CAuthManager::NeedsAuth() {
	return false;
}

const CAuthToken CAuthManager::Authenticate(const std::string& username, const std::string& password, const std::string& ip) {
	kode::db::AutoReset autoReset( m_pCheckLoginStatement );
	
	m_pCheckLoginStatement->Bind( "@Username", username );
	if( m_pCheckLoginStatement->GetNextRow() ) {
		int nID = m_pCheckLoginStatement->GetColumn<int>( 0 );
		std::string passwordHash = m_pCheckLoginStatement->GetColumn<std::string>( 1 );	

		//
		// Now we need to confirm the passwords match
		//
		unsigned char hashBuffer[ CryptoPP::SHA::DIGESTSIZE ];
		CryptoPP::HMAC< CryptoPP::SHA >( m_salt, sizeof( m_salt ) )
			.CalculateDigest( 
				hashBuffer, 
				reinterpret_cast< const unsigned char* >( passwordHash.c_str() ), 
				passwordHash.length()
				);
		
		
		if( passwordHash != password )
			throw authentication_exception("Invalid username or password");
		return CAuthToken( username, nID, CAuthToken::AccessLevel_Admin );
	}
	throw authentication_exception("Invalid username or password" );
}

const CAuthToken CAuthManager::DefaultAuth()
{
	if( NeedsAuth() )
		throw authentication_exception( "Authentication required but no details provided" );
	return CAuthToken( "default", -1, CAuthToken::AccessLevel_Normal );
}

}
