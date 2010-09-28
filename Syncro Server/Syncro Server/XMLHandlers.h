#ifndef _XML_ACCEPT_HANDLER_H_
#define _XML_ACCEPT_HANDLER_H_

#include "ServerComms.h"
#include "XMLBuilder.h"

class CXMLAcceptHandler : public CAcceptHandler {
public:
	CXMLAcceptHandler();
	virtual ~CXMLAcceptHandler();

	virtual bool HandleAccept(CTCPConnection::TPointer inpNewConnection);
};

class CXMLRequestHandler : public CReceiveHandler { 
public:
	virtual ~CXMLRequestHandler() {};

	virtual bool CanHandleReceive(const TCharBuffer& inoBuffer);
	virtual bool HandleReceive(const TCharBuffer& inoBuffer);
	virtual bool CanRemove() { return m_fCloseConnection; };

	static CReceiveHandler::TPointer Create(CTCPConnection::TPointer inpConn) {
		CXMLRequestHandler* pNew = new CXMLRequestHandler(inpConn);
		return CReceiveHandler::TPointer( static_cast<CReceiveHandler*>(pNew) );
	}
protected:
	bool m_fCloseConnection;

	CTCPConnection::TPointer m_pConn;
	CXMLBuilder m_oXMLBuilder;

	CXMLRequestHandler(CTCPConnection::TPointer inpConn);
};

class CXMLSendHandler : public CSendHandler {
public:
	typedef boost::shared_ptr<CXMLSendHandler> TPointer;

	virtual ~CXMLSendHandler();

	virtual bool SendStarting() {
		//Fill out the size in the buffer
		size_t nBufferSize = m_aBuffer.size();
		if( nBufferSize < 5 ) 
			return false;
		*((long*)&m_aBuffer[1]) = ToJavaEndian<long>((long)nBufferSize);
		return true;
	}
	virtual void SendDone(int innSent);

	static TPointer Create(CTCPConnection::TPointer inpConn) {
		CXMLSendHandler* pNew = new CXMLSendHandler(inpConn);
		return TPointer( pNew );
	}

	template<typename tFunction>
	void CreateXML(tFunction oFunc) {
		oFunc(m_aBuffer);
	}

protected:

	CTCPConnection::TPointer m_pConn;

	CXMLSendHandler(CTCPConnection::TPointer inpConn);

private:
};

#endif 