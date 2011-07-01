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

#include "ServerComms.h"
#include "HandshakeHandlers.h"
#include <libsyncro/comms.h>
#include <boost/bind.hpp>
#include <iostream>

namespace syncro
{

using namespace boost::asio::ip;
using boost::asio::io_service;
using std::cout;

const int DEFAULT_RECV_BUFFER = 1000;

ServerComms::ServerComms( io_service& inoIOService, unsigned int port ) :
	m_oAcceptor( inoIOService, tcp::endpoint( tcp::v4(), port ) )
{
	StartAccept();
}

ServerComms::~ServerComms()
{

}


void ServerComms::StartAccept()
{
	TCPConnection::TPointer oNewConn = TCPConnection::CreateConnection( m_oAcceptor.io_service() );
	m_oAcceptor.async_accept( oNewConn->GetSocket(), boost::bind( &ServerComms::HandleAccept, this, oNewConn, boost::asio::placeholders::error ) );
}

void ServerComms::HandleAccept( TCPConnection::TPointer inoNewConn, const boost::system::error_code& inoError )
{
	if( !inoError )
	{
		bool fOK = false;
		for( TAcceptHandlerMap::iterator pAcceptHandler = m_oAcceptHandlers.begin(); pAcceptHandler != m_oAcceptHandlers.end(); pAcceptHandler++ )
		{
			if(( *pAcceptHandler )->HandleAccept( inoNewConn ) )
			{
				fOK = true;
				break;
			}
		}
		if( fOK )
			StartAccept();
	}
}

void ServerComms::AddAcceptHandler( BaseAcceptHandler::TPointer inoAcceptHandler )
{
	m_oAcceptHandlers.insert( inoAcceptHandler );
}

TCPConnection::TCPConnection( io_service& inoIOService ) : m_oSocket( inoIOService )
{

}

void TCPConnection::StartRecv( int inBytes )
{
	using namespace boost::asio;
	using boost::bind;

	if( inBytes != 0 )
		m_aBuffer.resize( inBytes );
	else if( m_aBuffer.size() == 0 )
		m_aBuffer.resize( DEFAULT_RECV_BUFFER );

//	m_nWaitingRecv = inBytes;
	async_read( m_oSocket, buffer( m_aBuffer ),
	            bind( &TCPConnection::IsRecvFinished, shared_from_this(), placeholders::error, placeholders::bytes_transferred ),
	            bind( &TCPConnection::FinishedRecv, shared_from_this(), placeholders::error, placeholders::bytes_transferred ) );
}

bool TCPConnection::IsRecvFinished( const boost::system::error_code& inoError, std::size_t innBytesSoFar )
{
	if( inoError )
	{
		cout << "Error when checking received:  Received " << innBytesSoFar << " bytes\n";
		cout << inoError.message() << "\n";
		return true;
	}
	TCharBuffer oBuffer( m_aBuffer, innBytesSoFar );
	TRecvHandlerMap::iterator pRecv;
	for( pRecv = m_oRecvHandlers.begin(); pRecv != m_oRecvHandlers.end(); pRecv++ )
	{
		if( pRecv->second->CanHandleReceive( oBuffer ) )
			break;
	}
	if( pRecv != m_oRecvHandlers.end() )
	{
		m_pSelectedRecvHandler = pRecv->second;
		return true;
	}
	return false;
}

void TCPConnection::FinishedRecv( const boost::system::error_code& inoError, std::size_t innBytes )
{
	bool fatalError = false;
	if( inoError )
	{
		cout << "Error when receiving:  Received " << innBytes << " bytes\n";
		cout << inoError.message() << "\n";
		return;
	}
	else
	{
#ifdef COMMS_DEBUG
		cout << "Received " << innBytes << "\n";
#endif 

		TCharBuffer oBuffer( m_aBuffer, innBytes );

		if( !m_pSelectedRecvHandler )
		{
			cout << "ERROR: FinishedRecv called with no recv handler selected\n";
			fatalError = true;
		}
		else
		{
			m_pSelectedRecvHandler->HandleReceive( oBuffer );
			if( m_pSelectedRecvHandler->FatalError() )
				fatalError = true;
			m_pSelectedRecvHandler.reset();
		}
	}
	TRecvHandlerMap::iterator pRecv;
	for( pRecv = m_oRecvHandlers.begin(); pRecv != m_oRecvHandlers.end(); pRecv++ )
	{
		if( fatalError || pRecv->second->CanRemove() )
		{
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

void TCPConnection::Send( SendHandler::TPointer inpSendHandler )
{
	using namespace boost::asio;
	using boost::bind;

	if( m_pSendHandler.get() != NULL )
		throw std::logic_error( "Attempted 2 send operations on 1 TCP socket at once!" );

	if( inpSendHandler->SendStarting() )
	{
		m_pSendHandler = inpSendHandler;

		async_write( m_oSocket, buffer( inpSendHandler->GetBuffer().aBuffer ),
		             bind( &TCPConnection::IsSendFinished, shared_from_this(), placeholders::error, placeholders::bytes_transferred ),
		             bind( &TCPConnection::FinishedSend, shared_from_this(), placeholders::error, placeholders::bytes_transferred ) );
	}
};

bool
TCPConnection::IsSendFinished( const boost::system::error_code& inoError, std::size_t innBytesSoFar )
{
	if( inoError )
	{
		cout << "Error when sending: Sent " << innBytesSoFar << " bytes\n";
		cout << inoError.message() << "\n";
		return true;
	}
	return m_pSendHandler->HandleSend( innBytesSoFar );
}

void
TCPConnection::FinishedSend( const boost::system::error_code& inoError, std::size_t innBytes )
{
	if( inoError )
	{
		cout << "Error when sending:  Received " << innBytes << " bytes\n";
		cout << inoError.message() << "\n";
		return;
	}
	SendHandler::TPointer pHandler = m_pSendHandler;
	m_pSendHandler.reset();
	pHandler->SendDone( innBytes );

#ifdef COMMS_DEBUG
	cout << "Sent " << innBytes << " OK!";
#endif
}

std::string
TCPConnection::ClientIP()
{
	boost::system::error_code error;
	std::string rv(	m_oSocket.remote_endpoint().address().to_string( error ) );
	if( error )
		return "";
	else
		return rv;
}

const TCharBuffer SendHandler::GetBuffer()
{
	return TCharBuffer( m_aBuffer, m_aBuffer.size() );
}

};
