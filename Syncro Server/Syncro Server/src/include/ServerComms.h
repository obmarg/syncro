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

#ifndef _SERVER_COMMS_H_
#define _SERVER_COMMS_H_

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <map>
#include <set>
#include <kode/utils.h>
#include "common.h"

namespace syncro
{

class CReceiveHandler
{
public:
	typedef boost::shared_ptr<CReceiveHandler> TPointer;

	CReceiveHandler() : m_fatalError( false ) {};
	virtual ~CReceiveHandler() {};

	virtual bool CanHandleReceive( const TCharBuffer& inoBuffer ) = 0;
	virtual bool HandleReceive( const TCharBuffer& inoBuffer ) = 0;
	virtual bool CanRemove() = 0;
	virtual bool FatalError()
	{
		return m_fatalError;
	};
protected:
	bool m_fatalError;
};

class CSendHandler
{
public:
	typedef boost::shared_ptr< CSendHandler > TPointer;

	virtual bool SendStarting()
	{
		return true;
	};
	virtual bool HandleSend( int innSent )
	{
		if( innSent >= ( int )m_aBuffer.size() )
			return true;
		return false;
	};
	virtual void SendDone( int innSent ) = 0;

	virtual const TCharBuffer GetBuffer();
protected:

	TCharBuffer::TBuff m_aBuffer;
};

class CTCPConnection : public boost::enable_shared_from_this<CTCPConnection>
{
public:
	typedef boost::shared_ptr<CTCPConnection> TPointer;

	static TPointer CreateConnection( boost::asio::io_service& inoIOService )
	{
		return TPointer( new CTCPConnection( inoIOService ) );
	};

	void AddRecvHandler( CReceiveHandler::TPointer inpHandler, int innPriority )
	{
		m_oRecvHandlers.insert( std::make_pair( innPriority, inpHandler ) );
	}
	void StartRecv( int inBytes );
	bool IsRecvFinished( const boost::system::error_code& inoError, std::size_t innBytesSoFar );
	void FinishedRecv( const boost::system::error_code& inoError, std::size_t innBytes );

	void Send( CSendHandler::TPointer inpSendHandler );

	bool IsSendFinished( const boost::system::error_code& inoError, std::size_t innBytesSoFar );
	void FinishedSend( const boost::system::error_code& inoError, std::size_t innBytes );

	boost::asio::ip::tcp::socket& GetSocket()
	{
		return m_oSocket;
	};

	std::string		ClientIP();

private:
	CTCPConnection( boost::asio::io_service& inoIOService );

	boost::asio::ip::tcp::socket m_oSocket;
	TCharBuffer::TBuff m_aBuffer;

	typedef std::multimap<int, CReceiveHandler::TPointer> TRecvHandlerMap;
	TRecvHandlerMap m_oRecvHandlers;
	CReceiveHandler::TPointer m_pSelectedRecvHandler;

	CSendHandler::TPointer m_pSendHandler;
};

class BaseAcceptHandler
{
public:
	typedef boost::shared_ptr<BaseAcceptHandler> TPointer;

	BaseAcceptHandler( int innPriority ) : m_nPriority( innPriority ) {};
	virtual ~BaseAcceptHandler() {};

	virtual bool HandleAccept( CTCPConnection::TPointer inpNewConnection ) = 0;
	bool operator<( const BaseAcceptHandler& inoRHS )
	{
		if( m_nPriority < inoRHS.m_nPriority )
			return true;
		return false;
	}

protected:
	int m_nPriority;
};

class ServerComms
{
public:
	ServerComms( boost::asio::io_service& inoIOService );
	~ServerComms();

	void AddAcceptHandler( BaseAcceptHandler::TPointer inoAcceptHandler );
private:
	ServerComms( const ServerComms& inoServerComms );
	boost::asio::ip::tcp::acceptor m_oAcceptor;

	typedef std::multiset<BaseAcceptHandler::TPointer, kode::utils::CPointerLessThan> TAcceptHandlerMap;
	TAcceptHandlerMap m_oAcceptHandlers;

	void StartAccept();
	void HandleAccept( CTCPConnection::TPointer inpConnection, const boost::system::error_code& inoError );
};

};		//namespace syncro

#endif
