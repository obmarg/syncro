#include "SyncroPBResponseFactory.h"
#include "BinaryDataResponse.h"
#include "BinaryDataRequest.h"

namespace syncro {

CBasePBResponse::TPointer CSyncroPBResponseFactory::CreateResponse(const pb::PacketHeader& inoPacketHeader,const TCharBuffer::TBuff& inoSubpackets) {
	switch( inoPacketHeader.packet_type() ) {
	case eSyncroPBPacketTypes_BinaryRequest: {
		CBinaryDataRequest oRequest(inoPacketHeader.subpacket_sizes_size(), inoSubpackets );
		m_pCurrentSendData.reset( new CFileSendData( oRequest.GetFilename() ) );
		}
		//Fall through
	case eSyncroPBPacketTypes_BinaryContinue:
		return CBasePBResponse::TPointer( new CBinaryDataResponse( (*m_pCurrentSendData) ) );
	}
	return CBasePBResponse::TPointer();
}


};	//namespace syncro