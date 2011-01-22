#include "BinaryDataResponse.h"

namespace syncro
{

CBinaryDataResponse::CBinaryDataResponse( CFileSendData& inoFileData )
	: m_oFileData( inoFileData )
{
	m_oPacketHeader.set_file_offset( m_oFileData.GetFilePosition() );
	m_oPacketHeader.set_file_size( m_oFileData.GetFileSize() );
	m_oPacketHeader.set_hash_size( 0 );
	if( inoFileData.IsStartFile() )
		m_oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_START );
	else
		m_oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_MIDDLE );

	if( m_oFileData.IsFileFinishedAfterChunk( m_oFileData.GetChunkSize() ) )
		m_oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_END );
}

CBinaryDataResponse::~CBinaryDataResponse()
{

}

unsigned int CBinaryDataResponse::GetSubpacketCount()
{
	return 2;
}

uint32_t CBinaryDataResponse::GetSubpacketSize( uint32_t subpacket )
{
	assert( subpacket == 0 || subpacket == 1 );
	if( subpacket == 0 )
	{
		return m_oPacketHeader.ByteSize();
	}
	else if( subpacket == 1 )
	{
		return m_oFileData.GetChunkSize();
	}
	throw std::exception();
}



void CBinaryDataResponse::WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream )
{
	if( inSubpacketIndex == 0 )
		WriteMessage( m_oPacketHeader, stream );
	else
	{
		m_oFileData.FillBuffer( stream );
	}
}

};		//namespace syncro
