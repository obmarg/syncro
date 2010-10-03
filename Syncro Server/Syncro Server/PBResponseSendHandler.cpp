#include "PBResponseSendHandler.h"
#include <vector>

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
	oResponseHeader.set_packettype( m_pResponse->GetPacketType() );

	vector<unsigned int> aPacketSizes = m_pResponse->GetSubpacketSizes();
	unsigned int nTotalPacketSize = 0;
	foreach( unsigned int nPacketSize, aPacketSizes ) {
		nTotalPacketSize += nPacketSize;
		oResponseHeader.add_subpacketsizes( nPacketSize );
	}
	unsigned int nPBHeaderSize = oResponseHeader.ByteSize();
	m_aBuffer.reserve( nTotalPacketSize + nPBHeaderSize + nHeadSize );
	m_aBuffer.resize( nPBHeaderSize + nHeadSize );
	
	m_aBuffer[0] = PB_RESPONSE_FIRST_BYTE;
	*((int*)(&m_aBuffer[1])) = ToJavaEndian( nPBHeaderSize );
	oResponseHeader.SerializeToArray(&m_aBuffer[nHeadSize], nPBHeaderSize );
	for( int nSubpacket =0; nSubpacket < m_pResponse->GetSubpacketCount(); nSubpacket++ ) {
		m_pResponse->WriteSubpacket( nSubpacket, std::back_inserter( m_aBuffer ) );
	}

	return true;
}

void CPBResponseSendHandler::SendDone(int innSent) {
	//TODO: Do we need to do anything here?  don't think so....
}

} //namespace syncro