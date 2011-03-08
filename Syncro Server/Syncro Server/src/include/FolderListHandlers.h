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

#ifndef __FOLDER_LIST_HANDLERS_H_
#define __FOLDER_LIST_HANDLERS_H_

#include "common.h"
#include "BasePBResponse.h"
#include <libsyncro/packet_types.h>
#include <libsyncro/folderlist.h>
#include <libsyncro/protocol_buffers/folders.pb.h>

namespace syncro {

namespace pbHandlers {

class FolderListResponse : public BasePBResponse
{
public:
	FolderListResponse( const FolderList& list );

	virtual uint32_t GetSubpacketSize(uint32_t subpacket)
	{
		assert( subpacket == 0 );
		return m_response.ByteSize();
	}
	virtual unsigned int GetSubpacketCount()
	{ return 1; };

	virtual unsigned int GetPacketType()
	{ return comms::packet_types::FolderListResponse; }

	virtual void WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream )
	{
		if( inSubpacketIndex != 0 )
			throw std::logic_error( "Attempted to write imaginary subpacket in FolderListResponse::WriteSubpacket" );
		WriteMessage( m_response, stream );
	}
private:
	pb::FolderList m_response;
};

}	// namespace pbHandlers
}	// namespace syncro

#endif
