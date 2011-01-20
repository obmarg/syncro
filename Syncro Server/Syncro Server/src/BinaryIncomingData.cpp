#include "BinaryIncomingData.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>
#include <boost/numeric/conversion/cast.hpp>

namespace syncro
{

CBinaryIncomingData::CBinaryIncomingData(
    const std::string insFilename,
    VoidCallback completedCallback
) :
	m_oFile( insFilename.c_str(), std::ios::out | std::ios::binary ),
	m_completedCallback( completedCallback ),
	m_dataTransferred( 0 )
{

}

CBinaryIncomingData::~CBinaryIncomingData()
{
}

void
CBinaryIncomingData::HandlePacket( TInputStreamList& inaInputStreams )
{
	using boost::numeric_cast;

	if( !m_oFile.is_open() )
	{
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
		m_oFile.write( reinterpret_cast< const char* >( pData ), nSize );
		m_dataTransferred += nSize;
	}
	if( oPacket.binary_packet_type() == pb::BinaryPacketHeader_SectionType_END )
	{
		m_oFile.close();
		int64_t timeMs = m_stopwatch.GetMS();
		float dataTransferred = numeric_cast<float>( m_dataTransferred ) / 1024.0f;
		float timeSecs = numeric_cast<float>( timeMs ) / 1000.0f;
		float rate = dataTransferred / timeSecs;
		std::cout << "Finished receiving file (" << rate << "kb/s)\n";
		if( m_completedCallback )
		{
			m_completedCallback();
		}
	}
}

}
