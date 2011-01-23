/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

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