#include "HandshakeHandlers.h"
#include "XMLHandlers.h"
#include "FileRequestHandler.h"

#include <vector>
#include <string>
#include <iterator>

namespace syncro {

using std::vector;
using std::string;

const std::string CHandshakeRecv::m_sRecvString = "Hello Syncro?";

CHandshakeRecv::CHandshakeRecv(CTCPConnection::TPointer inpConn) : m_pConn(inpConn) {
	m_fFoundString = false;
}

CHandshakeRecv::~CHandshakeRecv() {
}


bool CHandshakeRecv::CanHandleReceive(const TCharBuffer& inoBuffer) {
	if( inoBuffer.nSize >= m_sRecvString.length() ) {
		vector<char> oRecv( m_sRecvString.begin(),m_sRecvString.end() );
		if( std::equal(m_sRecvString.begin(),m_sRecvString.end(),inoBuffer.aBuffer.begin(), std::equal_to<char>() ) )
			return true;
	}
	return false;
}

bool CHandshakeRecv::HandleReceive(const TCharBuffer& inoBuffer) {
	if( inoBuffer.nSize >= m_sRecvString.length() ) {
		vector<char> oRecv( m_sRecvString.begin(),m_sRecvString.end() );
		if( std::equal(m_sRecvString.begin(),m_sRecvString.end(),inoBuffer.aBuffer.begin(), std::equal_to<char>() ) ) {
			m_fFoundString = true;
			m_pConn->Send( CHandshakeResponse::Create(m_pConn) );
			return true;
		}
	}
	return false;
}

bool CHandshakeRecv::CanRemove() {
	return m_fFoundString;
}

CHandshakeResponse::CHandshakeResponse(CTCPConnection::TPointer inpConn) : m_pConn(inpConn) {
	m_aBuffer.push_back(100);
	m_aBuffer.push_back(118);
	m_aBuffer.push_back(50);
}

CHandshakeResponse::~CHandshakeResponse() {

}

bool CHandshakeResponse::HandleSend(int innSent) {
	if( innSent >= (int)m_aBuffer.size() )
		return true;
	return false;
}

void CHandshakeResponse::SendDone(int innSent) {
	m_pConn->AddRecvHandler( CXMLRequestHandler::Create(m_pConn) , 0 );
	m_pConn->AddRecvHandler( CFileRequestHandler::Create(m_pConn), 0 );
	m_pConn->StartRecv( 0 );
}

};		//namespace syncro