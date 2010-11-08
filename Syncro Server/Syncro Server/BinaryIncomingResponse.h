#ifndef _BINARY_INCOMING_RESPONSE_H_
#define _BINARY_INCOMING_RESPONSE_H_

#include "common.h"
#include "VectorPBResponse.h"
#include "SyncroPBResponseFactory.h"
#include "FileSendData.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace syncro {

class CBinaryIncomingResponse : public CBasePBResponse {
public:

	enum eResponseType{
		eResponseType_Response,
		eResponseType_Ack
	};

	static CBasePBResponse::TPointer Create(eResponseType eType, bool infValue) {
		CBasePBResponse::TPointer oPointer( new CBinaryIncomingResponse( eType, infValue ) );
		return oPointer;
	}

	virtual ~CBinaryIncomingResponse() {};

	virtual std::vector<unsigned int> GetSubpacketSizes() {
		std::vector<unsigned int> oRV(1);
		oRV[0] = m_pMessage->ByteSize();
		return oRV;
	}
	virtual unsigned int GetSubpacketCount() {
		return 1;
	}

	virtual void WriteSubpacket(int inSubpacketIndex,google::protobuf::io::ZeroCopyOutputStream& stream) {
		if( inSubpacketIndex != 0 )
			throw std::logic_error( "BinaryIncomingResponse::WriteSubpacket called with imaginary subpacket index" );
		m_pMessage->SerializeToZeroCopyStream( &stream );
	}

	virtual unsigned int GetPacketType() {
		if( m_eType == eResponseType_Response ) {
			return eSyncroPBPacketTypes_BinaryIncomingResponse;
		} else {
			return eSyncroPBPacketTypes_BinaryIncomingDataAck;
		}
	};
protected:

	CBinaryIncomingResponse(eResponseType eType, bool infValue) {
		m_eType = eType;
		switch( eType ) {
		case eResponseType_Response: {
			m_pMessage.reset( new pb::BinaryIncomingResponse() );
			pb::BinaryIncomingResponse* pResponse = dynamic_cast<pb::BinaryIncomingResponse*>( m_pMessage.get() );
			pResponse->set_accepted(infValue);
			break;
		}
		case eResponseType_Ack: {
			m_pMessage.reset( new pb::BinaryIncomingAck );
			pb::BinaryIncomingAck* pResponse = dynamic_cast<pb::BinaryIncomingAck*>( m_pMessage.get() );
			pResponse->set_ok(infValue);
			break;
		}
		default:
			throw std::logic_error( "Invalid type passed to CBinaryIncomingResponse constructor" );
		}
	}

	boost::scoped_ptr<google::protobuf::MessageLite> m_pMessage;
	eResponseType m_eType;
};

};
#endif