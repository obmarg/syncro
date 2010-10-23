#include "BinaryDataResponse.h"

namespace syncro {

CBinaryDataResponse::CBinaryDataResponse(CFileSendData& inoFileData) 
	: m_oFileData(inoFileData) 
{
	m_oPacketHeader.set_file_offset( m_oFileData.GetFilePosition() );
	m_oPacketHeader.set_file_size( m_oFileData.GetFileSize() );
	m_oPacketHeader.set_hash_size( 0 );
	if( inoFileData.IsStartFile() )
		m_oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_START );
	else
		m_oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_MIDDLE );
	
	if( m_oFileData.IsFileFinishedAfterChunk( m_oFileData.GetChunkSize() )  )
		m_oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_END );
}

CBinaryDataResponse::~CBinaryDataResponse() {

}

unsigned int CBinaryDataResponse::GetSubpacketCount() {
	return 2;
}

std::vector<unsigned int> CBinaryDataResponse::GetSubpacketSizes() {
	std::vector<unsigned int> rv;
	rv.push_back( m_oPacketHeader.ByteSize() );
	rv.push_back( m_oFileData.GetChunkSize() );
	return rv;
}



void CBinaryDataResponse::WriteSubpacket(int inSubpacketIndex,google::protobuf::io::ZeroCopyOutputStream& stream) {
	if( inSubpacketIndex == 0 )
		WriteMessage( m_oPacketHeader, stream );
	else {
		m_oFileData.FillBuffer( stream );
	}
}
	
};		//namespace syncro
