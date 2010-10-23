#include "PBResponseSendHandler.h"
#include <vector>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

namespace syncro {

const unsigned char PB_RESPONSE_FIRST_BYTE = 106;

CPBResponseSendHandler::CPBResponseSendHandler(CTCPConnection::TPointer inpConn,CBasePBResponse::TPointer inpResponse) : m_pConn(inpConn), m_pResponse(inpResponse){

};

CPBResponseSendHandler::~CPBResponseSendHandler() {

}

bool CPBResponseSendHandler::SendStarting() {
	using std::vector;

	unsigned int nHeadSize = sizeof( PB_RESPONSE_FIRST_BYTE ) + sizeof( unsigned int );

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
	
	m_aBuffer[0] = PB_RESPONSE_FIRST_BYTE;
	*((int*)(&m_aBuffer[1])) = ToJavaEndian( nPBHeaderSize );
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
	m_pConn->StartRecv( 0 );
}

} //namespace syncro