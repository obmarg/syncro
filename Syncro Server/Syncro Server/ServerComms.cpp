#include "ServerComms.h"
#include "HandshakeHandlers.h"
#include <libsyncro/comms.h>
#include <boost/bind.hpp>
#include <iostream>

namespace syncro {

using namespace boost::asio::ip;
using boost::asio::io_service;
using std::cout;

const int DEFAULT_RECV_BUFFER = 1000;

CServerComms::CServerComms(io_service& inoIOService) : 
	m_oAcceptor( inoIOService, tcp::endpoint(tcp::v4(), comms::SERVER_PORT) )
{
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
	bool fatalError = false;
	if( inoError ) {
		cout << "Error when receiving:  Received " << innBytes << " bytes\n";
		cout << inoError.message() << "\n";
		return;
	} else {
		cout << "Received " << innBytes << "\n";

		TCharBuffer oBuffer( m_aBuffer, innBytes );

		if( !m_pSelectedRecvHandler ) {
			cout << "ERROR: FinishedRecv called with no recv handler selected\n";
			fatalError = true;
		} else {
			m_pSelectedRecvHandler->HandleReceive(oBuffer);
			if( m_pSelectedRecvHandler->FatalError() )
				fatalError = true;
			m_pSelectedRecvHandler.reset();
		}
	}
	TRecvHandlerMap::iterator pRecv;
	for(pRecv = m_oRecvHandlers.begin(); pRecv != m_oRecvHandlers.end(); pRecv++ ) {
		if( fatalError || pRecv->second->CanRemove() ) {
			m_oRecvHandlers.erase( pRecv );
			pRecv = m_oRecvHandlers.begin();
			if( pRecv == m_oRecvHandlers.end() )
				break;
		}
	}
	if( fatalError )
	{
		m_oSocket.close();
	}
}

void CTCPConnection::Send(CSendHandler::TPointer inpSendHandler) {
	using namespace boost::asio;
	using boost::bind;

	if( m_pSendHandler.get() != NULL )
		throw std::logic_error("Attempted 2 send operations on 1 TCP socket at once!");

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

std::string
CTCPConnection::ClientIP()
{
	boost::system::error_code error;
	std::string rv(	m_oSocket.remote_endpoint().address().to_string(error) );
	if( error )
		return "";
	else
		return rv;
}

const TCharBuffer CSendHandler::GetBuffer() {
	return TCharBuffer( m_aBuffer,m_aBuffer.size() );
}

};
