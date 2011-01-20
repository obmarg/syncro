#ifndef _BINARY_DATA_RESPONSE_H_
#define _BINARY_DATA_RESPONSE_H_

#include "common.h"
#include "VectorPBResponse.h"
#include "SyncroPBResponseFactory.h"
#include "FileSendData.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>
#include <libsyncro/packet_types.h>

namespace syncro
{

class CBinaryDataResponse : public CBasePBResponse
{
public:
	CBinaryDataResponse( CFileSendData& inoFileData );
	virtual ~CBinaryDataResponse();

	virtual std::vector<unsigned int> GetSubpacketSizes();
	virtual unsigned int GetSubpacketCount();

	virtual void WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream );

	virtual unsigned int GetPacketType()
	{
		return comms::packet_types::BinaryResponse;
	};
protected:
	pb::BinaryPacketHeader m_oPacketHeader;
	CFileSendData& m_oFileData;
};

};		//namespace syncro

#endif