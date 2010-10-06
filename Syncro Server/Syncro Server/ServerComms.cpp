#include "ServerComms.h"

#include <boost/bind.hpp>
#include <iostream>
#include "HandshakeHandlers.h"

namespace syncro {

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
		bool fOK = false;
		for(TAcceptHandlerMap::iterator pAcceptHandler = m_oAcceptHandlers.begin(); pAcceptHandler != m_oAcceptHandlers.end(); pAcceptHandler++ ) {
			if( (*pAcceptHandler)->HandleAccept( inoNewConn ) ) {
				fOK = true;
				break;
			}
		}
		if( fOK )
			StartAccept();
	}
}

void CServerComms::AddAcceptHandler(CAcceptHandler::TPointer inoAcceptHandler) {
	m_oAcceptHandlers.insert( inoAcceptHandler );
}

CTCPConnection::CTCPConnection(io_service& inoIOService) : m_oSocket(inoIOService) {

}

void CTCPConnection::StartRecv(int inBytes) {

	using namespace boost::asio;
	using boost::bind;

	if( inBytes == 0 ) 
		inBytes = DEFAULT_RECV_BUFFER;
	m_aBuffer.resize( inBytes );
//	m_nWaitingRecv = inBytes;
	async_read( m_oSocket, buffer( m_aBuffer ), 
		bind(&CTCPConnection::IsRecvFinished, shared_from_this(), placeholders::error, placeholders::bytes_transferred ),
		bind(&CTCPConnection::FinishedRecv, shared_from_this(), placeholders::error, placeholders::bytes_transferred ) );
}

bool CTCPConnection::IsRecvFinished(const boost::system::error_code& inoError, std::size_t innBytesSoFar) {
	if( inoError ) {
		cout << "Error when checking received:  Received " << innBytesSoFar << " bytes\n";
		cout << inoError.message() << "\n";
		return true;
	}
	TCharBuffer oBuffer( m_aBuffer, innBytesSoFar );
	TRecvHandlerMap::iterator pRecv;
	for(pRecv = m_oRecvHandlers.begin(); pRecv != m_oRecvHandlers.end(); pRecv++ ) {
		if( pRecv->second->CanHandleReceive( oBuffer ) )
			break;
	}
	if( pRecv != m_oRecvHandlers.end() ) {
		m_pSelectedRecvHandler = pRecv->second;
		return true;
	}
	return false;
}

void CTCPConnection::FinishedRecv(const boost::system::error_code& inoError, std::size_t innBytes ) {
	if( inoError ) {
		cout << "Error when receiving:  Received " << innBytes << " bytes\n";
		cout << inoError.message() << "\n";
		return;
	} else {
		cout << "Received " << innBytes << "\n";

		TCharBuffer oBuffer( m_aBuffer, innBytes );

		m_pSelectedRecvHandler->HandleReceive(oBuffer);
		m_pSelectedRecvHandler.reset();
	}
	TRecvHandlerMap::iterator pRecv;
	for(pRecv = m_oRecvHandlers.begin(); pRecv != m_oRecvHandlers.end(); pRecv++ ) {
		if( pRecv->second->CanRemove() ) {
			m_oRecvHandlers.erase( pRecv );
			pRecv = m_oRecvHandlers.begin();
			if( pRecv == m_oRecvHandlers.end() )
				break;
		}
	}
}

void CTCPConnection::Send(CSendHandler::TPointer inpSendHandler) {
	using namespace boost::asio;
	using boost::bind;

	if( m_pSendHandler.get() != NULL )
		throw std::exception("Attempted 2 send operations on 1 TCP socket at once!");

	if( inpSendHandler->SendStarting() ) {
		m_pSendHandler = inpSendHandler;
	
		async_write( m_oSocket, buffer( inpSendHandler->GetBuffer().aBuffer ), 
			bind(&CTCPConnection::IsSendFinished, shared_from_this(), placeholders::error, placeholders::bytes_transferred ),
			bind(&CTCPConnection::FinishedSend, shared_from_this(), placeholders::error, placeholders::bytes_transferred ) );
	}
};

bool 
CTCPConnection::IsSendFinished(const boost::system::error_code& inoError, std::size_t innBytesSoFar) {
	if( inoError ) {
		cout << "Error when sending: Sent " << innBytesSoFar << " bytes\n";
		cout << inoError.message() << "\n";
		return true;
	}
	return m_pSendHandler->HandleSend(innBytesSoFar);
}

void 
CTCPConnection::FinishedSend(const boost::system::error_code& inoError, std::size_t innBytes) {
	if( inoError ) {
		cout << "Error when sending:  Received " << innBytes << " bytes\n";
		cout << inoError.message() << "\n";
		return;
	}
	CSendHandler::TPointer pHandler = m_pSendHandler;
	m_pSendHandler.reset();
	pHandler->SendDone(innBytes);
	
	cout << "Sent " << innBytes << " OK!";
}

const TCharBuffer CSendHandler::GetBuffer() {
	return TCharBuffer( m_aBuffer,m_aBuffer.size() );
}

};