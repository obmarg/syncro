#include "BinaryDataResponse.h"
#include "protocol_buffers\binarydata.pb.h"

namespace syncro {

CBinaryDataResponse::CBinaryDataResponse(CFileSendData& inoFileData) {
	CVectorPBResponse::TSubpacketPtr pHeader( new TCharBuffer::TBuff() );
	pb::BinaryPacketHeader oPacketHeader;
	oPacketHeader.set_file_offset( inoFileData.GetFilePosition() );
	oPacketHeader.set_file_size( inoFileData.GetFileSize() );
	oPacketHeader.set_hash_size( 0 );
	if( inoFileData.IsStartFile() )
		oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_START );
	else
		oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_MIDDLE );
	
	CVectorPBResponse::TSubpacketPtr pFileChunk( new TCharBuffer::TBuff() );
	inoFileData.FillBuffer( (*pFileChunk) );
	if( inoFileData.IsFileFinished() )
		oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_END );
	
	pHeader->resize( oPacketHeader.ByteSize() );
	oPacketHeader.SerializeToArray( &(*pHeader)[0], pHeader->size() );
	
	m_aSubpackets.push_back( pHeader );
	m_aSubpackets.push_back( pFileChunk );

}

CBinaryDataResponse::~CBinaryDataResponse() {

}

};		//namespace syncro