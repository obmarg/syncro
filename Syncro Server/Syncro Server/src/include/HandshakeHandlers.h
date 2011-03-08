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

#ifndef _HANDSHAKE_HANDLERS_H_
#define _HANDSHAKE_HANDLERS_H_

#include "ServerComms.h"
#include "BasePBResponse.h"
#include "common.h"
#include "VectorPBResponse.h"
#include <libsyncro/protocol_buffers/handshake.pb.h>

namespace syncro {
namespace pbHandlers {

class HandshakeRequest
{
public:
	HandshakeRequest( const InputStreamList& inaInputStreams );
	const int GetMajorVersion() const
	{
		return m_nMajorVersion;
	};
	const int GetMinorVersion() const
	{
		return m_nMinorVersion;
	};
	bool HasAuthDetails() const
	{
		return m_fHasAuth;
	};
	const std::string GetUsername() const
	{
		return m_UserName;
	};
	const std::string GetPassword() const
	{
		return m_Password;
	};

	BasePBResponse::TPointer GetResponse();
private:
	int m_nMajorVersion;
	int m_nMinorVersion;
	bool m_fHasAuth;
	std::string m_UserName;
	std::string m_Password;

	const static std::string m_sRecvString;
};

class HandshakeResponse : public BasePBResponse
{
public:
	HandshakeResponse();

	virtual uint32_t GetSubpacketSize(uint32_t subpacket);
	virtual unsigned int GetSubpacketCount()
	{
		return 1;
	};

	virtual unsigned int GetPacketType();

	virtual void WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream );
private:
	pb::HandshakeResponse m_oMessage;
};

class CSaltResponse : public CVectorPBResponse
{
public:
	static BasePBResponse::TPointer Create( const std::string& salt )
	{
		BasePBResponse::TPointer pointer( new CSaltResponse( salt ) );
		return pointer;
	}
	unsigned int GetPacketType();

private:
	CSaltResponse( const std::string& salt );
};

}	// namespace pbHandlers
}	// namespace syncro

#endif