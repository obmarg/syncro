#include "BinaryIncomingData.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>

namespace syncro {

CBinaryIncomingData::CBinaryIncomingData( const std::string insFilename ) : m_oFile(insFilename.c_str(), std::ios::out | std::ios::binary ) {

}

CBinaryIncomingData::~CBinaryIncomingData() {
}

void
CBinaryIncomingData::HandlePacket(TInputStreamList& inaInputStreams) {
	if( m_oFile.is_open() ) {
		throw std::runtime_error( "CBinaryIncomingData::HandlePacket called, but destination file is not open" );
	}
	pb::BinaryPacketHeader oPacket;
	if( inaInputStreams.size() != 2 )
		throw std::runtime_error( "Invalid number of streams passed to CBinaryIncomingData::HandlePacket" );
	oPacket.ParseFromZeroCopyStream( inaInputStreams[0] );
	//TODO: Get Packet Size?
	const void* pData = NULL;
	int nSize = 0;
	while( inaInputStreams[1]->Next( &pData, &nSize ) )
	{
		m_oFile.write( reinterpret_cast< const char* >(pData), nSize );
	}
	if( oPacket.binary_packet_type() == pb::BinaryPacketHeader_SectionType_END )
		m_oFile.close();
}

}
