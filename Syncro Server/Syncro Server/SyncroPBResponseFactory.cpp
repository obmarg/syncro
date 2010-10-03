#include "SyncroPBResponseFactory.h"
#include "BinaryDataResponse.h"
#include "BinaryDataRequest.h"

namespace syncro {

CBasePBResponse::TPointer CSyncroPBResponseFactory::CreateResponse(const unsigned int innPacketType, TInputStreamList& inaInputStreams) {
	
	switch( innPacketType ) {
	case eSyncroPBPacketTypes_BinaryRequest: {
		CBinaryDataRequest oRequest( inaInputStreams );
		m_pCurrentSendData.reset( new CFileSendData( oRequest.GetFilename() ) );
		}
		//Fall through
	case eSyncroPBPacketTypes_BinaryContinue:
		return CBasePBResponse::TPointer( new CBinaryDataResponse( (*m_pCurrentSendData) ) );
	}
	return CBasePBResponse::TPointer();
}


};	//namespace syncro