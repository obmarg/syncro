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

#ifndef _BINARY_DATA_RESPONSE_H_
#define _BINARY_DATA_RESPONSE_H_

#include "common.h"
#include "VectorPBResponse.h"
#include "SyncroPBResponseFactory.h"
#include "FileSendData.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>
#include <libsyncro/packet_types.h>

namespace syncro {
namespace pbHandlers {

class CBinaryDataResponse : public BasePBResponse
{
public:
	CBinaryDataResponse( FileSendData& inoFileData );
	virtual ~CBinaryDataResponse();

	virtual uint32_t GetSubpacketSize(uint32_t subpacket);
	virtual unsigned int GetSubpacketCount();

	virtual void WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream );

	virtual unsigned int GetPacketType()
	{
		return comms::packet_types::BinaryResponse;
	};
protected:
	pb::BinaryPacketHeader m_packetHeader;
	FileSendData& m_fileData;
};

}	// namespace pbHandlers
}	// namespace syncro

#endif