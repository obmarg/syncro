#ifndef _FILE_SEND_HANDLER_H_
#define _FILE_SEND_HANDLER_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include "ServerComms.h"

class CFileSendHandler : CSendHandler {
public:
	typedef boost::shared_ptr<CFileSendHandler> TPointer;

	virtual ~CFileSendHandler();

	virtual void SendDone(int innSent);

	static TPointer Create(CTCPConnection::TPointer inpConn,std::string insFilename) {
		CFileSendHandler* pNew = new CFileSendHandler(inpConn,insFilename);
		return TPointer( pNew );
	}

protected:

	CTCPConnection::TPointer m_pConn;

	CFileSendHandler(CTCPConnection::TPointer inpConn,std::string insFilename);

private:
};

#endif