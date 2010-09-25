#ifndef _XML_ACCEPT_HANDLER_H_
#define _XML_ACCEPT_HANDLER_H_

#include "ServerComms.h"

class CXMLAcceptHandler : public CAcceptHandler {
public:
	CXMLAcceptHandler();
	virtual ~CXMLAcceptHandler();

	virtual bool HandleAccept(CTCPConnection::TPointer inpNewConnection);
};

class CXMLReceiveHandler : public CReceiveHandler { 
public:
	virtual ~CXMLReceiveHandler();

	virtual bool CanHandleReceive(const TCharBuffer& inoBuffer);
	virtual bool HandleReceive(const TCharBuffer& inoBuffer);
	virtual bool CanRemove() { return m_fCloseConnection; };

	static CReceiveHandler::TPointer Create(CTCPConnection::TPointer inpConn) {
		CXMLReceiveHandler* pNew = new CXMLReceiveHandler(inpConn);
		return CReceiveHandler::TPointer( static_cast<CReceiveHandler*>(pNew) );
	}
protected:
	bool m_fCloseConnection;

	CTCPConnection::TPointer m_pConn;

	CXMLReceiveHandler(CTCPConnection::TPointer inpConn);
};

#endif 