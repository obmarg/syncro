#include "PBRequestHandler.h"
#include "PBResponseSendHandler.h"

namespace syncro {

using std::string;

const unsigned char PB_REQUEST_FIRST_BYTE = 105;

CPBRequestHandler::CPBRequestHandler(CTCPConnection::TPointer inpConn,CBasePBResponseFactory::TPointer inpResponseFactory) : m_pConn( inpConn ), m_pResponseFactory(inpResponseFactory) {
	m_fCloseConnection = false;
	ResetVariables();		//don't know if this is needed, but can't hurt much
}

void CPBRequestHandler::ResetVariables() {
	m_oHeader.Clear();
	m_nBufferReadSoFar = 0;
}

bool CPBRequestHandler::CanHandleReceive(const TCharBuffer& inoBuffer) {
	unsigned int nHeadSize = sizeof( PB_REQUEST_FIRST_BYTE ) + sizeof( unsigned int );

	if( (m_nBufferReadSoFar > 0) && m_oHeader.IsInitialized() ) {
		//We have read in a header already, so we're just waiting for the rest of the data
		int nTotalSubpacketSize = 0;
		for(int nSubPacket=0; nSubPacket < m_oHeader.subpacket_sizes_size(); nSubPacket++ ) {
			nTotalSubpacketSize += m_oHeader.subpacket_sizes(nSubPacket);
		}
		if( inoBuffer.nSize > (m_nBufferReadSoFar+nTotalSubpacketSize) )
			return true;
	}
	else if(inoBuffer.nSize > nHeadSize ) {
		if( inoBuffer.aBuffer[0] == PB_REQUEST_FIRST_BYTE  ) {
			unsigned int nExpectedHeaderSize = ( *(int*)(&inoBuffer.aBuffer[1]) );
			nExpectedHeaderSize = FromJavaEndian(nExpectedHeaderSize);
			if( inoBuffer.nSize >= nExpectedHeaderSize+nHeadSize ) {
				m_oHeader.ParseFromArray( (void*)inoBuffer.aBuffer[ nHeadSize ], nExpectedHeaderSize );
				if( !m_oHeader.IsInitialized() )
					throw std::exception( "Received invalid/incomplete PB Request Header" );
			}
		}
	}
	return false;
}

bool CPBRequestHandler::HandleReceive(const TCharBuffer& inoBuffer) {

	TCharBuffer::TBuff aSubpackets( inoBuffer.begin() + m_nBufferReadSoFar, inoBuffer.end() );

	CBasePBResponse::TPointer pResponse = m_pResponseFactory->CreateResponse( m_oHeader, aSubpackets );

	m_pConn->Send( CPBResponseSendHandler::Create(m_pConn,pResponse) );

	ResetVariables();
	return true;
};

};		//namespace syncro