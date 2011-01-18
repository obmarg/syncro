#ifndef SYNCRO_SIMPLE_PB_RESPONSE_H_
#define SYNCRO_SIMPLE_PB_RESPONSE_H_

#include "BasePBResponse.h"
#include <google/protobuf/message_lite.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <exception>

namespace syncro {

class SimplePBResponse : public CBasePBResponse {
public:
	typedef boost::shared_ptr< google::protobuf::MessageLite > MessagePtr;
	
	SimplePBResponse( unsigned int messageType, MessagePtr message ) :
	m_messageType( messageType ),
	m_message( message )
	{

	}
	virtual ~SimplePBResponse() {};

	virtual std::vector<unsigned int> GetSubpacketSizes() {
		std::vector< unsigned int > rv(1);
		rv[0] = m_message->ByteSize();
		return rv;
	}

	virtual unsigned int GetSubpacketCount() {
		return 1;
	};

	virtual void WriteSubpacket(int inSubpacketIndex,google::protobuf::io::ZeroCopyOutputStream& stream)
	{
		if( inSubpacketIndex != 0 )
			throw std::logic_error( "Invalid subpacket index in SimplePBResponse::WriteSubpacket" );
	}

	virtual unsigned int GetPacketType() { 
		return m_messageType;
	}
private:
	unsigned int m_messageType;
	MessagePtr m_message;
};

}	// namespace syncro

#endif