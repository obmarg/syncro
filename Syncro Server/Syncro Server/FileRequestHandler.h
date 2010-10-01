#ifndef _FILE_REQUEST_H_
#define _FILE_REQUEST_H_

#include "common.h"
#include "ServerComms.h"
#include "FileSendData.h"

class CFileRequestHandler : public CReceiveHandler { 
public:
	virtual ~CFileRequestHandler();

	virtual bool CanHandleReceive(const TCharBuffer& inoBuffer);
	virtual bool HandleReceive(const TCharBuffer& inoBuffer);
	virtual bool CanRemove() { return m_fCloseConnection; };

	static CFileRequestHandler::TPointer Create(CTCPConnection::TPointer inpConn) {
		CFileRequestHandler* pNew = new CFileRequestHandler(inpConn);
		return CReceiveHandler::TPointer( static_cast<CReceiveHandler*>(pNew) );
	}
protected:
	bool m_fCloseConnection;

	CTCPConnection::TPointer m_pConn;
	CFileSendData::TPointer m_pCurrentSend;

	CFileRequestHandler(CTCPConnection::TPointer inpConn);
};

#endif