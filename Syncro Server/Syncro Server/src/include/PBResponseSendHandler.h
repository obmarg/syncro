#ifndef _PB_RESPONSE_SEND_HANDLER_H_
#define _PB_RESPONSE_SEND_HANDLER_H_

#include "common.h"
#include "ServerComms.h"
#include "BasePBResponse.h"

namespace syncro
{

class CPBResponseSendHandler : public CSendHandler
{
public:
	virtual ~CPBResponseSendHandler();
	virtual bool SendStarting();	//Want to override this definetly (i think)

	static CSendHandler::TPointer Create( CTCPConnection::TPointer inpConn )
	{
		return CSendHandler::TPointer( new CPBResponseSendHandler( inpConn ) );
	}

	void SetPBResponse( CBasePBResponse::TPointer inpResponse )
	{
		m_pResponse = inpResponse;
	}

	virtual void SendDone( int innSent );
protected:

	CTCPConnection::TPointer m_pConn;

	CBasePBResponse::TPointer m_pResponse;

	CPBResponseSendHandler( CTCPConnection::TPointer inpConn );
};

} //namespace syncro;

#endif