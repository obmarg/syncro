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

#ifndef _XML_ACCEPT_HANDLER_H_
#define _XML_ACCEPT_HANDLER_H_

#include "ServerComms.h"
#include "XMLBuilder.h"

namespace syncro
{

class CXMLAcceptHandler : public CAcceptHandler
{
public:
	CXMLAcceptHandler();
	virtual ~CXMLAcceptHandler();

	virtual bool HandleAccept( CTCPConnection::TPointer inpNewConnection );
};

class CXMLRequestHandler : public CReceiveHandler
{
public:
	virtual ~CXMLRequestHandler() {};

	virtual bool CanHandleReceive( const TCharBuffer& inoBuffer );
	virtual bool HandleReceive( const TCharBuffer& inoBuffer );
	virtual bool CanRemove()
	{
		return m_fCloseConnection;
	};

	static CReceiveHandler::TPointer Create( CTCPConnection::TPointer inpConn )
	{
		CXMLRequestHandler* pNew = new CXMLRequestHandler( inpConn );
		return CReceiveHandler::TPointer( static_cast<CReceiveHandler*>( pNew ) );
	}
protected:
	bool m_fCloseConnection;

	CTCPConnection::TPointer m_pConn;
	CXMLBuilder m_oXMLBuilder;

	CXMLRequestHandler( CTCPConnection::TPointer inpConn );
};

class CXMLSendHandler : public CSendHandler
{
public:
	typedef boost::shared_ptr<CXMLSendHandler> TPointer;

	virtual ~CXMLSendHandler();

	virtual bool SendStarting();
	virtual void SendDone( int innSent );

	static TPointer Create( CTCPConnection::TPointer inpConn )
	{
		CXMLSendHandler* pNew = new CXMLSendHandler( inpConn );
		return TPointer( pNew );
	}

	template<typename tFunction>
	void CreateXML( tFunction oFunc )
	{
		oFunc( m_aBuffer );
	}

protected:

	CTCPConnection::TPointer m_pConn;

	CXMLSendHandler( CTCPConnection::TPointer inpConn );

private:
};

};	//namespace syncro

#endif