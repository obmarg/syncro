#ifndef _HANDSHAKE_HANDLERS_H_
#define _HANDSHAKE_HANDLERS_H_

#include "ServerComms.h"
#include "BasePBResponse.h"
#include "common.h"
#include "protocol_buffers/handshake.pb.h"

namespace syncro {

class CPBHandshakeRequest {
public:
	CPBHandshakeRequest(TInputStreamList& inaInputStreams);
	const int GetMajorVersion() const { return m_nMajorVersion; };
	const int GetMinorVersion() const { return m_nMinorVersion; };

	CBasePBResponse::TPointer GetResponse();
private:
	int m_nMajorVersion;
	int m_nMinorVersion;

	const static std::string m_sRecvString;
};

class CPBHandshakeResponse : public CBasePBResponse {
public:
	CPBHandshakeResponse();

	virtual std::vector<unsigned int> GetSubpacketSizes();
	virtual unsigned int GetSubpacketCount() { return 1; };

	virtual unsigned int GetPacketType();

	virtual void WriteSubpacket(int inSubpacketIndex,google::protobuf::io::ZeroCopyOutputStream& stream);
private:
	pb::HandshakeResponse m_oMessage;
};

};	//namespace syncro

#endif