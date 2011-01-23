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

CFileSendData::CFileSendData(
    const std::string& insFilename,
    const int innRequestedBufferSize,
    const VoidCallback& completionCallback,
    int64_t fileStartOffset
)
	:
	m_sFilename( insFilename ),
	m_completionCallback( completionCallback ),
	m_finishedAfterChunk( false )
{
	m_nRequestedBufferSize = innRequestedBufferSize;
	OpenFile( fileStartOffset );
}

CFileSendData::~CFileSendData()
{

}

void CFileSendData::OpenFile( int64_t fileStartOffset )
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
	cout << m_sFilename.c_str() << "\n";

	m_oFile.open( m_sFilename.c_str() , ios::in | ios::binary );
	if( m_oFile.is_open() )
	{
		m_oFile.seekg( 0, ios::end );
		m_nFileSize = m_oFile.tellg();
		m_oFile.seekg( fileStartOffset, ios::beg );
	}
	else
		throw std::runtime_error( "CFileSendData called on non existant file" );
}

void CFileSendData::FillBuffer( google::protobuf::io::ZeroCopyOutputStream& stream )
{
	int nReadAmount = GetChunkSize();

	void* pData;
	int nSize;
	while( nReadAmount > 0 )
	{
		if( !stream.Next( &pData, &nSize ) )
			throw std::runtime_error( "ZeroCopyOutputStream returned false in CFileSendData::FillBuffer" );
		char* pChars = reinterpret_cast< char* >( pData );
		//TODO: Maybe start using google's fileinputstream to read data?
		m_oFile.read( pChars, nSize );
		nReadAmount -= nSize;
	}
	if( m_finishedAfterChunk )
	{
		CallCompletionCallback();
	}
}

unsigned int CFileSendData::GetChunkSize()
{
	int nBufferSize = DEFAULT_FILE_SEND_BUFFER_SIZE;
	if( m_nRequestedBufferSize != 0 )
	{
		int nRequestedBufferSize = ( m_nRequestedBufferSize - 1024 );		//Take away 1k just to be safe?
		nBufferSize = std::min( DEFAULT_FILE_SEND_BUFFER_SIZE, nRequestedBufferSize );
	}
	//TODO: maybe stop doing the above -1024, and find a better way of doing it
	int64_t nSizeLeft = m_nFileSize - m_oFile.tellg();
	int64_t rv = std::min(
	                 boost::numeric_cast<int64_t>( nBufferSize ),
	                 nSizeLeft
	             );
	return boost::numeric_cast<unsigned int>( rv );
}

bool CFileSendData::IsStartFile()
{
	if( m_oFile.tellg() == ( std::streamoff )0 )
		return true;
	return false;
}

bool CFileSendData::IsFileFinished()
{

	if( m_oFile.tellg() == m_nFileSize || m_oFile.eof() )
	{
		CallCompletionCallback();

		return true;
	}
	return false;
}

std::istream::pos_type CFileSendData::GetFilePosition()
{
	return m_oFile.tellg();
}

bool CFileSendData::IsFileFinishedAfterChunk( unsigned int inNextChunkSize )
{
	std::streamoff nNextTell = m_oFile.tellg() + ( std::streamoff )inNextChunkSize;
	if( nNextTell == m_nFileSize )
	{
		m_finishedAfterChunk = true;
		return true;
	}
	return false;
}

void CFileSendData::CallCompletionCallback()
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
		std::cout << "Error when calculating transfer rate in CFileSendData::CallCompletionCallback:\n"
		          << ex.what();
	}

	if( m_completionCallback )
	{
		m_completionCallback();
		m_completionCallback.clear();
	}
}

};		//namespace syncro
