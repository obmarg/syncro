#define _SCL_SECURE_NO_WARNINGS

#include "HandshakeHandlers.h"
#include "XMLHandlers.h"
#include "PBRequestHandler.h"

#include <vector>
#include <string>
#include <iterator>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include "SyncroDB.h"

namespace syncro {

using std::vector;
using std::string;

const std::string CHandshakeRecv::m_sRecvString = "Hello Syncro?";

CBasePBResponseFactory::TPointer CHandshakeResponse::ms_pPBResponseFactory;

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
	m_aBuffer.push_back(':');

	Database::TPointer oDB = CSyncroDB::OpenDB();
	vector<unsigned char> aUUID( 16 );
	try {
		std::string sUUID = oDB->runScalar<std::string>("SELECT uuid FROM ServerID");
		if( sUUID.length() != 16 )
			throw std::exception( "Invalid legnth of UUID returned from database in CHandshakeResponse" );
		std::copy( sUUID.begin(), sUUID.end(), aUUID.begin() );
	}catch( const std::out_of_range& ) {
		boost::uuids::uuid oUUID( ( boost::uuids::random_generator()() ) );
		if( oUUID.size() != 16 )
			throw std::exception( "Invalid legnth of UUID generated in CHandshakeResponse" );
		for( int n=0; n<oUUID.size(); n++ ) {
			aUUID[n] = oUUID.data[n];
		}
		std::string sUUID = std::string( aUUID.begin(), aUUID.end() );
		oDB->run("INSERT INTO ServerID(uuid) VALUES('" + sUUID + "');");
	}
	std::copy( aUUID.begin(), aUUID.end(), back_inserter( m_aBuffer ) );
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
	/*m_pConn->AddRecvHandler( CFileRequestHandler::Create(m_pConn), 0 );*/
	m_pConn->AddRecvHandler( CPBRequestHandler::Create(m_pConn, ms_pPBResponseFactory ), 0 );
	m_pConn->StartRecv( 0 );
}

};		//namespace syncro