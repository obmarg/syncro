#include "PBResponseSendHandler.h"
#include <libsyncro/comms.h>
#include <libsyncro/packet_types.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>
#include <iostream>

namespace syncro
{

// Uncomment to enable PB packet debug output
//#define PB_PACKET_DEBUG

CPBResponseSendHandler::CPBResponseSendHandler( CTCPConnection::TPointer inpConn ) : m_pConn( inpConn )
{

};

CPBResponseSendHandler::~CPBResponseSendHandler()
{

}

bool CPBResponseSendHandler::SendStarting()
{
	using std::vector;
	using boost::numeric_cast;
	using namespace syncro::comms;

	if( !m_pResponse )
		throw std::logic_error( "PB Send Starting, but PBResponseSendHandler has not been assigned any data" );

	//TODO: Make this stuff use libsyncro's PacketHeader stuff at some point
	unsigned int nHeadSize = comms::PacketHeader::BYTE_SIZE;

	pb::PacketHeader oResponseHeader;
	oResponseHeader.set_packet_type( m_pResponse->GetPacketType() );
#ifdef PB_PACKET_DEBUG
	packet_types::ePBPacketTypes packetType =
	    numeric_cast< packet_types::ePBPacketTypes >( m_pResponse->GetPacketType() );
	std::cout <<
	          "Sending " <<
	          packet_types::Str( packetType ) <<
	          " packet\n";
#endif

	vector<unsigned int> aPacketSizes = m_pResponse->GetSubpacketSizes();
	unsigned int nTotalPacketSize = 0;
	foreach( unsigned int nPacketSize, aPacketSizes )
	{
		nTotalPacketSize += nPacketSize;
		oResponseHeader.add_subpacket_sizes( nPacketSize );
	}
	unsigned int nPBHeaderSize = oResponseHeader.ByteSize();
	m_aBuffer.resize( nTotalPacketSize + nPBHeaderSize + nHeadSize );

	m_aBuffer[0] = comms::PB_RESPONSE_FIRST_BYTE;
	*(( int* )( &m_aBuffer[1] ) ) = kode::utils::ToJavaEndian( nPBHeaderSize );
	oResponseHeader.SerializeToArray( &m_aBuffer[nHeadSize], nPBHeaderSize );
	unsigned int nWrittenSoFar = nPBHeaderSize + nHeadSize;

	for( uint32_t nSubpacket = 0; nSubpacket < m_pResponse->GetSubpacketCount(); nSubpacket++ )
	{
		google::protobuf::io::ArrayOutputStream oStream( &m_aBuffer[ nWrittenSoFar ], aPacketSizes[ nSubpacket ], aPacketSizes[ nSubpacket ] );
		m_pResponse->WriteSubpacket( nSubpacket, oStream );
		nWrittenSoFar += aPacketSizes[ nSubpacket ];
	}

	return true;
}

void CPBResponseSendHandler::SendDone( int innSent )
{
	//TODO: maybe don't start recv if there are no current recv handlers.  add checks for this

	uint32_t recvBufferSize = 0;

	if( m_pResponse->NextRecvBufferSize() != 0 )
		recvBufferSize =
		    m_pResponse->NextRecvBufferSize()
		    + comms::PacketHeader::BYTE_SIZE
		    + 1024;

	m_pConn->StartRecv( recvBufferSize );

	m_pResponse.reset( );
}

} //namespace syncro