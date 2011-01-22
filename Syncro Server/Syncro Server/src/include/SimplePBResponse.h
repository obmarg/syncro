#ifndef SYNCRO_SIMPLE_PB_RESPONSE_H_
#define SYNCRO_SIMPLE_PB_RESPONSE_H_

#include "BasePBResponse.h"
#include <google/protobuf/message_lite.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <exception>

namespace syncro
{

class SimplePBResponse : public CBasePBResponse
{
public:
	typedef boost::shared_ptr< google::protobuf::MessageLite > MessagePtr;

	SimplePBResponse( unsigned int messageType, MessagePtr message ) :
		m_messageType( messageType ),
		m_message( message )
	{

	}
	virtual ~SimplePBResponse() {};

	virtual uint32_t GetSubpacketSize(uint32_t subpacket)
	{
		assert( subpacket == 0 );
		return m_message->ByteSize();
	}

	virtual unsigned int GetSubpacketCount()
	{
		return 1;
	};

	virtual void WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream )
	{
		if( inSubpacketIndex != 0 )
			throw std::logic_error( "Invalid subpacket index in SimplePBResponse::WriteSubpacket" );
		WriteMessage( *m_message, stream );
	}

	virtual unsigned int GetPacketType()
	{
		return m_messageType;
	}
private:
	unsigned int m_messageType;
	MessagePtr m_message;
};

}	// namespace syncro

#endif