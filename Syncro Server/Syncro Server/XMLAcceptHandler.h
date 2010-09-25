#ifndef _XML_ACCEPT_HANDLER_H_
#define _XML_ACCEPT_HANDLER_H_

#include "ServerComms.h"

class CXMLAcceptHandler : public CAcceptHandler {
public:
	CXMLAcceptHandler();
	virtual ~CXMLAcceptHandler();

	virtual bool HandleAccept(CTCPConnection::TPointer inpNewConnection);
};

#endif 