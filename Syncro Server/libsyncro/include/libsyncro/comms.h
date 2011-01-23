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

#ifndef _SYNCRO_COMMS_H_
#define _SYNCRO_COMMS_H_

#include <kode/utils.h>

namespace syncro
{
namespace comms
{

const int SERVER_PORT = 9998;

const unsigned int BROADCAST_SERVER_PORT = 9995;
const unsigned int BROADCAST_RESPONSE_PORT = 9996;
const std::string BROADCAST_DISCOVERY_STRING = "SyncroHELLO";
const std::string BROADCAST_RESPONSE_PREFIX = "HELLO: ";

const unsigned char PB_REQUEST_FIRST_BYTE = 105;
const unsigned char PB_RESPONSE_FIRST_BYTE = 106;

const std::string HANDSHAKE_REQUEST_MAGIC =		"Hello Syncro?";
const std::string HANDSHAKE_RESPONSE_MAGIC =	"Hey bitch!";

class PacketHeader
{
public:
	PacketHeader()
	{

	}
	~PacketHeader()
	{

	}

	enum { BYTE_SIZE = sizeof( PB_REQUEST_FIRST_BYTE ) + sizeof( unsigned int ) };

	template<class tByteArray>
	void Read( const tByteArray& input )
	{
		m_firstByte = input[0];
		m_packetSize = ( *( unsigned int* )( &input[1] ) );
		m_packetSize = kode::utils::FromJavaEndian( m_packetSize );
	}

	template<class tByteArray>
	void Write( tByteArray& output )
	{
		output[0] = m_firstByte;
		*(( unsigned int* )( &output[1] ) ) = kode::utils::ToJavaEndian( m_packetSize );
	}

	PacketHeader& SetFirstByte( const unsigned char byte )
	{
		m_firstByte = byte;
		return ( *this );
	}
	PacketHeader& SetPacketSize( const unsigned int packetSize )
	{
		m_packetSize = packetSize;
		return ( *this );
	}
	unsigned char FirstByte()
	{
		return m_firstByte;
	}
	unsigned int PacketSize()
	{
		return m_packetSize;
	}

private:
	unsigned char m_firstByte;
	unsigned int m_packetSize;
};

}
}

#endif