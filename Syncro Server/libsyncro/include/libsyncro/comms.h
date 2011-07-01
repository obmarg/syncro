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

//!	The default port a syncro server listens on
const int SERVER_PORT = 9998;

//!	The port a syncro broadcast server listens on
const unsigned int BROADCAST_SERVER_PORT = 9995;

//!	The port a syncro client should listen for broadcast responses on
const unsigned int BROADCAST_RESPONSE_PORT = 9996;

//!	The string to be sent as part of broadcast messages
const std::string BROADCAST_DISCOVERY_STRING = "SyncroHELLO";

//! The expected response to broadcast messages
const std::string BROADCAST_RESPONSE_PREFIX = "HELLO: ";

//! The first byte of a protocol buffer request
const unsigned char PB_REQUEST_FIRST_BYTE = 105;

//!	The first byte of a protocol buffer response
const unsigned char PB_RESPONSE_FIRST_BYTE = 106;

//!	The magic string sent by the client as part of a handshake
const std::string HANDSHAKE_REQUEST_MAGIC =		"Hello Syncro?";

//!	The magic response sent by the server as part of a handshake
const std::string HANDSHAKE_RESPONSE_MAGIC =	"Hey bitch!";

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	A class to handle reading & writing of syncro packet headers
//!
///////////////////////////////////////////////////////////////////////////////
class PacketHeader
{
public:
	PacketHeader()
	{

	}
	~PacketHeader()
	{

	}

	//! Constant defining the byte size of a packet header
	enum { BYTE_SIZE = sizeof( PB_REQUEST_FIRST_BYTE ) + sizeof( unsigned int ) };

	//!
	//! \brief 	Reads a packet header from a byte array
	//!
	//!	\tparam	tByteArray	The type of byte array
	//!
	//!	\param	input	The byte array to read from
	//!
	template<class tByteArray>
	void Read( const tByteArray& input )
	{
		m_firstByte = input[0];
		m_packetSize = ( *( unsigned int* )( &input[1] ) );
		m_packetSize = kode::utils::FromJavaEndian( m_packetSize );
	}

	//!
	//! \brief 	Writes a packet header to a byte array
	//!
	//!	\tparam	tByteArray	The type of byte array
	//!
	//!	\param	input	The byte array to write to
	//!
	template<class tByteArray>
	void Write( tByteArray& output )
	{
		output[0] = m_firstByte;
		*(( unsigned int* )( &output[1] ) ) = kode::utils::ToJavaEndian( m_packetSize );
	}

	//!
	//! \brief 	Sets the identification byte of the packet header
	//!			To differentiate between types of packet
	//!
	//!	\param	byte	The byte to use
	//!
	//!	\return	A reference to this. For chaining purposes
	//!
	PacketHeader& SetFirstByte( const unsigned char byte )
	{
		m_firstByte = byte;
		return ( *this );
	}

	//!
	//! \brief 	Sets the size of the packet
	//!
	//!	\param	packetSize	The size of the packet 
	//!
	//!	\return	A reference to this. For chaining purposes
	//!
	PacketHeader& SetPacketSize( const unsigned int packetSize )
	{
		m_packetSize = packetSize;
		return ( *this );
	}

	//!
	//!	\brief	Accessor for the current first byte
	//!
	//!	\return	The first byte of this packet header
	//!
	unsigned char FirstByte()
	{
		return m_firstByte;
	}

	//!
	//!	\brief	Accessor for the size of the packet
	//!
	//!	\return	The size of the packet
	//!
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
