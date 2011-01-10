#ifndef _PROTOCOL_BUFFER_REQUEST_HANDLER_H_
#define _PROTOCOL_BUFFER_REQUEST_HANDLER_H_

#include "common.h"
#include "ServerComms.h"
#include <libsyncro/protocol_buffers/header.pb.h>
#include <iterator>
#include "BasePBResponse.h"

namespace syncro {

class CPBRequestHandler : public CReceiveHandler {
public:
	virtual ~CPBRequestHandler() {};

	virtual bool CanHandleReceive(const TCharBuffer& inoBuffer);
	virtual bool HandleReceive(const TCharBuffer& inoBuffer);
	virtual bool CanRemove() { return m_fCloseConnection; };

	static CReceiveHandler::TPointer Create(CTCPConnection::TPointer inpConn,CBasePBResponseFactory::TPointer inpResponseFactory) {
		return CReceiveHandler::TPointer( static_cast<CReceiveHandler*>( new CPBRequestHandler(inpConn,inpResponseFactory) ) );
	}
protected:
	bool m_fCloseConnection;

	CTCPConnection::TPointer m_pConn;
	CBasePBResponseFactory::TPointer m_pResponseFactory;

	CSendHandler::TPointer m_pSendHandler;
	
	pb::PacketHeader m_oHeader;
	unsigned int m_nBufferReadSoFar;

	CPBRequestHandler(CTCPConnection::TPointer inpConn,CBasePBResponseFactory::TPointer inpResponseFactory);
	
	void ResetVariables();
};

};		//namespace syncro

#endif
