#include "PBResponseSendHandler.h"

namespace syncro {

CPBResponseSendHandler::CPBResponseSendHandler(CTCPConnection::TPointer inpConn,CBasePBResponse::TPointer inpResponse) : m_pConn(inpConn), m_pResponse(inpResponse){

};

CPBResponseSendHandler::~CPBResponseSendHandler() {

}

bool CPBResponseSendHandler::SendStarting() {
	return true;
}

void CPBResponseSendHandler::SendDone(int innSent) {
}

} //namespace syncro