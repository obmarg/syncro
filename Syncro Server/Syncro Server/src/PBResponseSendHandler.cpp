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

#include "PBResponseSendHandler.h"
#include "Logging.h"
#include <libsyncro/comms.h>
#include <libsyncro/packet_types.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>
#include <iostream>

namespace syncro
{

PBResponseSendHandler::PBResponseSendHandler( TCPConnection& connection ) : 
m_connection( connection )
{

};

PBResponseSendHandler::~PBResponseSendHandler()
{

}

bool PBResponseSendHandler::SendStarting()
{
	using std::vector;
	using boost::numeric_cast;
	using namespace syncro::comms;

	if( !m_pResponse )
		throw std::logic_error( "PB Send Starting, but PBResponseSendHandler has not been assigned any data" );

	unsigned int nHeadSize = comms::PacketHeader::BYTE_SIZE;

	pb::PacketHeader oResponseHeader;
	oResponseHeader.set_packet_type( m_pResponse->GetPacketType() );

	packet_types::ePBPacketTypes packetType =
	    numeric_cast< packet_types::ePBPacketTypes >( m_pResponse->GetPacketType() );
    log::debug <<
	          "Sending " <<
	          packet_types::Str( packetType ) <<
	          " packet\n";

	int numPackets = m_pResponse->GetSubpacketCount();
	uint32_t nTotalPacketSize = 0;
	for( int packetNum=0; packetNum < numPackets ; ++packetNum )
	{
		uint32_t packetSize = m_pResponse->GetSubpacketSize( packetNum );
		nTotalPacketSize += packetSize;
		oResponseHeader.add_subpacket_sizes( packetSize );
	}
	
	uint32_t nPBHeaderSize = oResponseHeader.ByteSize();
	m_aBuffer.resize( nTotalPacketSize + nPBHeaderSize + nHeadSize );

	m_aBuffer[0] = comms::PB_RESPONSE_FIRST_BYTE;
	*(( uint32_t* )( &m_aBuffer[1] ) ) = kode::utils::ToJavaEndian( nPBHeaderSize );
	oResponseHeader.SerializeToArray( &m_aBuffer[nHeadSize], nPBHeaderSize );
	unsigned int nWrittenSoFar = nPBHeaderSize + nHeadSize;

	for( uint32_t nSubpacket = 0; nSubpacket < m_pResponse->GetSubpacketCount(); nSubpacket++ )
	{
		uint32_t packetSize = oResponseHeader.subpacket_sizes( nSubpacket );
		google::protobuf::io::ArrayOutputStream oStream( 
			&m_aBuffer[ nWrittenSoFar ], 
			packetSize, 
			packetSize 
		);
		m_pResponse->WriteSubpacket( nSubpacket, oStream );
		nWrittenSoFar += packetSize;
	}

	return true;
}

void PBResponseSendHandler::SendDone( int innSent )
{
	uint32_t recvBufferSize = 0;

	if( m_pResponse->NextRecvBufferSize() != 0 )
		recvBufferSize =
		    m_pResponse->NextRecvBufferSize()
		    + comms::PacketHeader::BYTE_SIZE
		    + 1024;

	m_connection.StartRecv( recvBufferSize );

	m_pResponse.reset( );
}

} //namespace syncro
