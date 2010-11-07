#include "AuthManager.h"
#include "SyncroDB.h"

namespace syncro {

CAuthManager::CAuthManager() {
	m_pDB = CSyncroDB::OpenDB();
	m_pCheckLoginStatement = m_pDB->prepare("SELECT ID FROM Users WHERE UserName=@Username AND Password=@Password");
}

CAuthManager::~CAuthManager() {

}

bool CAuthManager::NeedsAuth() {
	return false;
}

const CAuthToken CAuthManager::Authenticate(std::string username, std::string password) {
	db::AutoReset autoReset( m_pCheckLoginStatement );
	m_pCheckLoginStatement->Bind( "@Username", username );
	m_pCheckLoginStatement->Bind( "@Password", password );
	if( m_pCheckLoginStatement->GetNextRow() ) {
		int nID = m_pCheckLoginStatement->GetColumn<int>( 0 );
		return CAuthToken( username, nID, CAuthToken::AccessLevel_Admin );
	}
	throw authentication_exception("Invalid username or password" );
}

}