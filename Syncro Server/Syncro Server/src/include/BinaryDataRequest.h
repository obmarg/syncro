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

#ifndef _BINARY_DATA_REQUEST_H_
#define _BINARY_DATA_REQUEST_H_

#include "common.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>
#include <stdint.h>


namespace syncro
{

class CBinaryDataRequest
{
public:
	CBinaryDataRequest( int folderId, const std::string& filename ) :
		m_sFilename( filename ),
		m_nFolderId( folderId ),
		m_nBufferSize( 0 ),
		m_nFileSize( -1 ),
		m_oneShot( false ),
		m_startOffset( 0 )
	{

	}

	CBinaryDataRequest( InputStreamList& inaInputStreams )
	{
		pb::BinaryDataRequest oRequest;
		if( inaInputStreams.size() == 1 )
		{
			if( !oRequest.ParseFromZeroCopyStream( inaInputStreams[0] ) )
				throw std::runtime_error( "CBinaryDataRequestHandler: invalid BinaryRequestData packet passed in" );
			m_sFilename = oRequest.file_name();
			m_nFolderId = oRequest.folder_id();
			m_nBufferSize = 0;
			if( oRequest.has_recv_buffer_size() )
				m_nBufferSize = oRequest.recv_buffer_size();
			if( oRequest.has_file_size() )
				m_nFileSize = oRequest.file_size();
			else
				m_nFileSize = -1;
			m_oneShot = false;
			if( oRequest.has_one_shot() )
				m_oneShot = oRequest.one_shot();
			m_startOffset = 0;
			if( oRequest.has_start_offset() )
				m_startOffset = oRequest.start_offset();
			//TODO: Add upload direction at some point?
		}
	}
	~CBinaryDataRequest() {};
	const std::string& GetFilename() const
	{
		return m_sFilename;
	};
	const int GetFolderId() const
	{
		return m_nFolderId;
	};
	const int GetBufferSize() const
	{
		return m_nBufferSize;
	};
	const int64_t GetFileSize() const
	{
		return m_nFileSize;
	};
	const bool IsOneShot() const
	{
		return m_oneShot;
	};
	const int64_t GetStartOffset() const
	{
		return m_startOffset;
	}
private:
	std::string m_sFilename;
	int m_nFolderId;
	int m_nBufferSize;
	int64_t m_nFileSize;
	bool m_oneShot;
	int64_t m_startOffset;
};

};	//namespace syncro

#endif
