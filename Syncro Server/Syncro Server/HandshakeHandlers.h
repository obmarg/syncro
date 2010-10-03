#ifndef _HANDSHAKE_HANDLERS_H_
#define _HANDSHAKE_HANDLERS_H_

#include "ServerComms.h"
#include "BasePBResponse.h"

namespace syncro {

class CHandshakeRecv : public CReceiveHandler {
public:
	virtual ~CHandshakeRecv();

	virtual bool CanHandleReceive(const TCharBuffer& inoBuffer);
	virtual bool HandleReceive(const TCharBuffer& inoBuffer);
	virtual bool CanRemove();

	static CReceiveHandler::TPointer Create(CTCPConnection::TPointer inpConn) {
		CHandshakeRecv* pNew = new CHandshakeRecv(inpConn);
		return CReceiveHandler::TPointer( static_cast<CReceiveHandler*>(pNew) );
	}

protected:
	bool m_fFoundString;
	const static std::string m_sRecvString;

	CTCPConnection::TPointer m_pConn;

	CHandshakeRecv(CTCPConnection::TPointer inpConn);
};

class CHandshakeResponse : public CSendHandler {
private:
public:
	typedef boost::shared_ptr<CHandshakeResponse> TPointer;

	virtual ~CHandshakeResponse();

	virtual bool HandleSend(int innSent);
	virtual void SendDone(int innSent);

	static TPointer Create(CTCPConnection::TPointer inpConn) {
		CHandshakeResponse* pNew = new CHandshakeResponse(inpConn);
		return TPointer( pNew );
	}

	static void SetPBResponseFactory( CBasePBResponseFactory::TPointer inpResponseFactory ) {
		ms_pPBResponseFactory = inpResponseFactory;
	};

protected:

	CTCPConnection::TPointer m_pConn;

	CHandshakeResponse(CTCPConnection::TPointer inpConn);

	static CBasePBResponseFactory::TPointer ms_pPBResponseFactory;

private:
};

};	//namespace syncro

#endif