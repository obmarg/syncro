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

#ifndef _BASE_PB_RESPONSE_H_
#define _BASE_PB_RESPONSE_H_


#include <libsyncro/protocol_buffers/header.pb.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <stdint.h>

namespace syncro
{

class CBasePBResponse
{
public:
	typedef boost::shared_ptr<CBasePBResponse> TPointer;

	CBasePBResponse() {};
	virtual ~CBasePBResponse() {};

	virtual uint32_t GetSubpacketSize(uint32_t subpacket) = 0;
	virtual unsigned int GetSubpacketCount() = 0;

	virtual unsigned int GetPacketType() = 0;

	virtual void WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream ) = 0;

	virtual void WriteMessage( const google::protobuf::MessageLite& inoMessage, google::protobuf::io::ZeroCopyOutputStream& stream )
	{
		inoMessage.SerializeToZeroCopyStream( &stream );
	}

	virtual uint32_t NextRecvBufferSize()
	{
		return 0;
	};
};

class CBasePBResponseFactory
{
public:
	virtual ~CBasePBResponseFactory() {};

	typedef boost::shared_ptr<CBasePBResponseFactory> TPointer;

	virtual CBasePBResponse::TPointer CreateResponse( const unsigned int innPacketType, InputStreamList& inaInputStreams ) = 0;
protected:
	CBasePBResponseFactory() {};
};

};	//namespace syncro
#endif
