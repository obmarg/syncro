/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FileSendData.h"

#include <boost/numeric/conversion/cast.hpp>
#include <stdint.h>
#include <iostream>

namespace syncro
{

const int DEFAULT_FILE_SEND_BUFFER_SIZE = 1024 * 1024;

const unsigned char FILE_SEND_FIRST_BYTE = 11;
const unsigned char FILE_SECTION_FIRST_BYTE = 16;
const unsigned char FILE_LAST_SECTION_FIRST_BYTE = 20;

FileSendData::FileSendData(
    const std::string& fileName,
    const int requestedBufferSize,
    const VoidCallback& completionCallback,
    int64_t fileStartOffset,
	int64_t fileModifiedTime
	) :
m_fileName( fileName ),
m_fileSize( 0 ),
m_completionCallback( completionCallback ),
m_finishedAfterChunk( false ),
m_fileModifiedTime( fileModifiedTime )
{
	m_nRequestedBufferSize = requestedBufferSize;
	OpenFile( fileStartOffset );
}

FileSendData::~FileSendData()
{

}

void FileSendData::OpenFile( int64_t fileStartOffset )
{
	using namespace std;
	
	if( fileStartOffset > 0 )
	{
		cout << "Resuming send of file: ";
	}
	else
	{
		cout << "Sending file: ";
	} 
	cout << m_fileName.c_str() << "\n";

	m_file.open( m_fileName.c_str() , ios::in | ios::binary );
	if( m_file.is_open() )
	{
		m_file.seekg( 0, ios::end );
		m_fileSize = m_file.tellg();
		m_file.seekg( fileStartOffset, ios::beg );
	}
	else
		throw std::runtime_error( "CFileSendData called on non existant file" );
}

void FileSendData::FillBuffer( 
	google::protobuf::io::ZeroCopyOutputStream& stream
	)
{
	int nReadAmount = GetChunkSize();

	void* pData;
	int nSize;
	while( nReadAmount > 0 )
	{
		if( !stream.Next( &pData, &nSize ) )
		{
			throw std::runtime_error( 
				"ZeroCopyOutputStream returned false in 
				CFileSendData::FillBuffer" 
				);
		}
		char* pChars = reinterpret_cast< char* >( pData );
		//TODO: Maybe start using google's fileinputstream to read data?
		m_file.read( pChars, nSize );
		nReadAmount -= nSize;
	}
	if( m_finishedAfterChunk )
	{
		CallCompletionCallback();
	}
}

unsigned int FileSendData::GetChunkSize()
{
	int nBufferSize = DEFAULT_FILE_SEND_BUFFER_SIZE;
	if( m_nRequestedBufferSize != 0 )
	{
		int nRequestedBufferSize = ( m_nRequestedBufferSize - 1024 );		//Take away 1k just to be safe?
		nBufferSize = 
			std::min( DEFAULT_FILE_SEND_BUFFER_SIZE, nRequestedBufferSize );
	}
	//TODO: maybe stop doing the above -1024, and find a better way of doing it
	int64_t nSizeLeft = m_fileSize - m_file.tellg();
	int64_t rv = std::min(
	                 boost::numeric_cast<int64_t>( nBufferSize ),
	                 nSizeLeft
	             );
	return boost::numeric_cast<unsigned int>( rv );
}

bool FileSendData::IsStartFile()
{
	if( m_file.tellg() == ( std::streamoff )0 )
		return true;
	return false;
}

bool FileSendData::IsFileFinished()
{

	if( m_file.tellg() == m_fileSize || m_file.eof() )
	{
		CallCompletionCallback();

		return true;
	}
	return false;
}

std::istream::pos_type FileSendData::GetFilePosition()
{
	return m_file.tellg();
}

bool FileSendData::IsFileFinishedAfterChunk( unsigned int inNextChunkSize )
{
	std::streamoff nNextTell = m_file.tellg() + ( std::streamoff )inNextChunkSize;
	if( nNextTell == m_fileSize )
	{
		m_finishedAfterChunk = true;
		return true;
	}
	return false;
}

void FileSendData::CallCompletionCallback()
{
	using boost::numeric_cast;

	try
	{
		int64_t timeMs = m_stopwatch.GetMS();
		float dataTransferred =
		    numeric_cast<float>( GetFilePosition() ) / 1024.0f;

		float timeSecs = numeric_cast<float>( timeMs ) / 1000.0f;

		float rate = dataTransferred / timeSecs;
		std::cout << "Finished sending file (" << rate << "kb/s)\n";
	}
	catch( const std::exception& ex )
	{
		std::cout << "Error when calculating transfer rate in "
				  << "CFileSendData::CallCompletionCallback:\n"
		          << ex.what();
	}

	if( m_completionCallback )
	{
		m_completionCallback();
		m_completionCallback.clear();
	}
}

};		//namespace syncro
