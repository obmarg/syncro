#include "PBResponseSendHandler.h"
#include <vector>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <libsyncro/comms.h>

namespace syncro {

CPBResponseSendHandler::CPBResponseSendHandler(CTCPConnection::TPointer inpConn) : m_pConn(inpConn) {

};

CPBResponseSendHandler::~CPBResponseSendHandler() {

}

bool CPBResponseSendHandler::SendStarting() {
	using std::vector;

	if( !m_pResponse )
		throw std::logic_error( "PB Send Starting, but PBResponseSendHandler has not been assigned any data" );

	//TODO: Make this stuff use libsyncro's PacketHeader stuff at some point
	unsigned int nHeadSize = comms::PacketHeader::BYTE_SIZE;

	pb::PacketHeader oResponseHeader;
	oResponseHeader.set_packet_type( m_pResponse->GetPacketType() );

	vector<unsigned int> aPacketSizes = m_pResponse->GetSubpacketSizes();
	unsigned int nTotalPacketSize = 0;
	foreach( unsigned int nPacketSize, aPacketSizes ) {
		nTotalPacketSize += nPacketSize;
		oResponseHeader.add_subpacket_sizes( nPacketSize );
	}
	unsigned int nPBHeaderSize = oResponseHeader.ByteSize();
	m_aBuffer.resize( nTotalPacketSize + nPBHeaderSize + nHeadSize );
	
	m_aBuffer[0] = comms::PB_RESPONSE_FIRST_BYTE;
	*((int*)(&m_aBuffer[1])) = kode::utils::ToJavaEndian( nPBHeaderSize );
	oResponseHeader.SerializeToArray(&m_aBuffer[nHeadSize], nPBHeaderSize );
	unsigned int nWrittenSoFar = nPBHeaderSize + nHeadSize;

	for( int nSubpacket =0; nSubpacket < m_pResponse->GetSubpacketCount(); nSubpacket++ ) {
		google::protobuf::io::ArrayOutputStream oStream( &m_aBuffer[ nWrittenSoFar ], aPacketSizes[ nSubpacket ], aPacketSizes[ nSubpacket ] );
		m_pResponse->WriteSubpacket( nSubpacket, oStream );
		nWrittenSoFar += aPacketSizes[ nSubpacket ];
	}

	return true;
}

void CPBResponseSendHandler::SendDone(int innSent) {
	m_pResponse.reset( );
	//TODO: maybe don't start recv if there are no current recv handlers.  add checks for this
	m_pConn->StartRecv( 0 );
}

} //namespace syncro