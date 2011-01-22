#include "BinaryIncomingData.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/filesystem.hpp>

namespace syncro
{

CBinaryIncomingData::CBinaryIncomingData(
    const std::string insFilename,
    VoidCallback completedCallback
) :
	m_filename( insFilename ),
	m_started(false),
	m_completedCallback( completedCallback ),
	m_dataTransferred( 0 )
{
}

CBinaryIncomingData::~CBinaryIncomingData()
{
	if( m_oFile.is_open() )
	{
		//If we wanted, we could call the fail callback in here...
		m_oFile.close();
	}
}

void
CBinaryIncomingData::HandlePacket( TInputStreamList& inaInputStreams )
{
	using boost::numeric_cast;

	if( !m_started )
	{
		StartFile();
	}

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

void CBinaryIncomingData::ShouldResume(int64_t resumePoint)
{
	StartFile( resumePoint );
}

void CBinaryIncomingData::StartFile(int64_t startOffset) 
{
	if( m_started )
	{
		throw std::logic_error( "Attempting to start a BinaryIncomingData that has already started" );
	}
	if( m_oFile.is_open() )
	{
		throw std::logic_error( "File already open, but started flag not set in "
			"CBinaryIncomingData::StartFile"
			);
	}

	m_started = true;

	std::ios_base::openmode openMode = 
		std::ios::out | std::ios::binary;

	if( startOffset != 0 )
	{
		int64_t destSize = 
			boost::numeric_cast<int64_t>( 
			boost::filesystem::file_size( m_filename )
			);
		if( startOffset > destSize )
		{
			throw std::runtime_error( 
				"Resume point > file size in "
				"CBinaryIncomingData::StartFile"
				);
		}
		openMode |= std::ios::app;
	}	

	m_oFile.open( m_filename.c_str(), openMode );
	m_oFile.seekp( startOffset );
	if( m_oFile.fail() )
	{
		throw std::runtime_error( 
			"File open/seek fail in CBinaryIncomingData::StartFile"
			);
	}

	if( startOffset > 0 )
	{
		std::cout << "Resuming receive of file: ";
	}
	else
	{
		std::cout << "Receiving file: ";
	} 
	std::cout << m_filename.c_str() << "\n";
}

}
