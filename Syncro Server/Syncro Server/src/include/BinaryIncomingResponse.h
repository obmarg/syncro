#ifndef _BINARY_INCOMING_RESPONSE_H_
#define _BINARY_INCOMING_RESPONSE_H_

#include "common.h"
#include "VectorPBResponse.h"
#include "SyncroPBResponseFactory.h"
#include "FileSendData.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>

namespace syncro
{

class CBinaryIncomingResponse : public CBasePBResponse
{
public:

	enum eResponseType
	{
		eResponseType_Response,
		eResponseType_Ack
	};

	static CBasePBResponse::TPointer Create( eResponseType eType, bool infValue, int64_t currentSize=0 )
	{
		CBasePBResponse::TPointer oPointer( new CBinaryIncomingResponse( eType, infValue, currentSize ) );
		return oPointer;
	}

	virtual ~CBinaryIncomingResponse() {};

	virtual uint32_t GetSubpacketSize(uint32_t subpacket)
	{
		assert( subpacket == 0 );
		return m_pMessage->ByteSize();
	}
	virtual unsigned int GetSubpacketCount()
	{
		return 1;
	}

	virtual void WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream )
	{
		if( inSubpacketIndex != 0 )
			throw std::logic_error( "BinaryIncomingResponse::WriteSubpacket called with imaginary subpacket index" );
		m_pMessage->SerializeToZeroCopyStream( &stream );
	}

	virtual unsigned int GetPacketType()
	{
		if( m_eType == eResponseType_Response )
		{
			return comms::packet_types::BinaryIncomingResponse;
		}
		else
		{
			return comms::packet_types::BinaryIncomingDataAck;
		}
	};

	virtual uint32_t NextRecvBufferSize()
	{
		return boost::numeric_cast<uint32_t>( m_recvBufferSize );
	}
protected:

	CBinaryIncomingResponse( eResponseType eType, bool infValue, int64_t currentSize=0 ) :
		m_recvBufferSize( 0 )
	{
		m_eType = eType;
		switch( eType )
		{
		case eResponseType_Response:
		{
			m_pMessage.reset( new pb::BinaryIncomingResponse() );
			pb::BinaryIncomingResponse* pResponse = dynamic_cast<pb::BinaryIncomingResponse*>( m_pMessage.get() );
			pResponse->set_accepted( infValue );
			m_recvBufferSize = 1024 * 51;
			pResponse->set_max_packet_size( m_recvBufferSize );
			pResponse->set_current_file_size( currentSize );
			break;
		}
		case eResponseType_Ack:
		{
			m_pMessage.reset( new pb::BinaryIncomingAck );
			pb::BinaryIncomingAck* pResponse = dynamic_cast<pb::BinaryIncomingAck*>( m_pMessage.get() );
			pResponse->set_ok( infValue );
			break;
		}
		default:
			throw std::logic_error( "Invalid type passed to CBinaryIncomingResponse constructor" );
		}
	}

	boost::scoped_ptr<google::protobuf::MessageLite> m_pMessage;
	eResponseType m_eType;
	int32_t m_recvBufferSize;
};

};
#endif