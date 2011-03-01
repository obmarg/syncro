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

#include "FolderListHandlers.h"
#include "SyncroDB.h"
#include "FolderMan.h"
#include <libsyncro/folderlist.h>
#include <libsyncro/packet_types.h>
#include <libsyncro/protocol_buffers/folders.pb.h>

namespace syncro
{

class FolderListResponse : public CBasePBResponse
{
public:
	FolderListResponse( const FolderList& list )
	{
		if( list.empty() )
		{
			m_response.set_empty( true );
		}
		else
		{
			foreach( const FolderInfo & folder, list )
			{
				pb::FolderInfo* info = m_response.add_folders();
				info->set_folder_id( folder.Id );
				info->set_folder_name( folder.Name );
				info->set_folder_path( folder.Path );
			}
		}
	}

	virtual uint32_t GetSubpacketSize(uint32_t subpacket)
	{
		assert( subpacket == 0 );
		return m_response.ByteSize();
	}
	virtual unsigned int GetSubpacketCount()
	{
		return 1;
	};

	virtual unsigned int GetPacketType()
	{
		return comms::packet_types::FolderListResponse;
	}

	virtual void WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream )
	{
		if( inSubpacketIndex != 0 )
			throw std::logic_error( "Attempted to write imaginary subpacket in FolderListResponse::WriteSubpacket" );
		WriteMessage( m_response, stream );
	}
private:
	pb::FolderList m_response;
};

FolderListRequestHandler::FolderListRequestHandler( InputStreamList& inaInputStreams )
{
	pb::FolderListRequest request;
	request.ParseFromZeroCopyStream( inaInputStreams[0] );
	//TODO: At some point start doing stuff with the request.  for now we just ignore everything
}

CBasePBResponse::TPointer FolderListRequestHandler::GetResponse()
{
	kode::db::Database::TPointer db = CSyncroDB::OpenDB( );
	CFolderMan folderMan( db );
	const FolderList& folders = folderMan.GetFoldersInfo();
	CBasePBResponse::TPointer rv( new FolderListResponse( folders ) );
	return rv;
}

}	// namespace syncro
