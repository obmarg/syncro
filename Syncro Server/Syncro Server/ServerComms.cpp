#include "ServerComms.h"

#include <boost/bind.hpp>
#include <iostream>

using namespace boost::asio::ip;
using boost::asio::io_service;
using std::cout;

const int SERVER_PORT = 9998;
const int DEFAULT_RECV_BUFFER = 1000;

CServerComms::CServerComms(io_service& inoIOService) : m_oAcceptor( inoIOService, tcp::endpoint(tcp::v4(), SERVER_PORT) ) {
	
	StartAccept();
}

CServerComms::~CServerComms() {

}


void CServerComms::StartAccept() {
	CTCPConnection::TPointer oNewConn = CTCPConnection::CreateConnection( m_oAcceptor.io_service() );
	m_oAcceptor.async_accept( oNewConn->GetSocket(), boost::bind(&CServerComms::HandleAccept, this, oNewConn, boost::asio::placeholders::error) );
}

void CServerComms::HandleAccept(CTCPConnection::TPointer inoNewConn, const boost::system::error_code& inoError) {
	if( !inoError ) {
		inoNewConn->StartRecv(0);
		StartAccept();
	}
}

CTCPConnection::CTCPConnection(io_service& inoIOService) : m_oSocket(inoIOService) {

}

void CTCPConnection::StartRecv(int inBytes) {
	if( inBytes == 0 ) 
		inBytes = DEFAULT_RECV_BUFFER;
	m_aBuffer.resize( inBytes );
	m_nWaitingRecv = inBytes;
	boost::asio::async_read( m_oSocket, boost::asio::buffer( m_aBuffer ), 
		boost::bind(&CTCPConnection::IsRecvFinished, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ),
		boost::bind(&CTCPConnection::FinishedRecv, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred ) );
}

bool CTCPConnection::IsRecvFinished(const boost::system::error_code& inoError, std::size_t innBytesSoFar) {
	if( inoError ) {
		cout << "Error when checking received:  Received " << innBytesSoFar << " bytes\n";
		return true;
	}
	m_nWaitingRecv -= innBytesSoFar;
	if( m_nWaitingRecv <= 0 ) {
		m_nWaitingRecv = 0;
		cout << "Finished Receiving\n";
		return true;
	}
	return false;
}

void CTCPConnection::FinishedRecv(const boost::system::error_code& inoError, std::size_t innBytes ) {
	if( inoError ) {
		cout << "Error when receiving:  Received " << innBytes << " bytes\n";
		return;
	} else {
		cout << "Received " << innBytes << "\n";
		StartRecv( );
	}
}