#ifndef _BASE_PB_RESPONSE_H_
#define _BASE_PB_RESPONSE_H_

#include <string>
#include "protocol_buffers\header.pb.h"
#include <boost\shared_ptr.hpp>

namespace syncro {

class CBasePBResponse {
public:
	typedef boost::shared_ptr<CBasePBResponse> TPointer;

	CBasePBResponse() {};
	virtual ~CBasePBResponse() {};

	virtual std::vector<unsigned int> GetSubpacketSizes()=0;
	virtual unsigned int GetSubpacketCount()=0;

	virtual unsigned int GetPacketType()=0;

	virtual void WriteSubpacket(int inSubpacketIndex,std::back_insert_iterator<TCharBuffer::TBuff> inoInsert)=0;

	virtual void WriteMessage(std::back_insert_iterator<TCharBuffer::TBuff> inoInsert, const google::protobuf::MessageLite& inoMessage ) {
		//TODO: start using array outputstreams for this shit
		std::string sOutput = inoMessage.SerializeAsString();
		std::copy( sOutput.begin(), sOutput.end(), inoInsert );
	}
};

class CBasePBResponseFactory {
public:
	virtual ~CBasePBResponseFactory() {};

	typedef boost::shared_ptr<CBasePBResponseFactory> TPointer;

	//virtual CBasePBResponse::TPointer CreateResponse(const pb::PacketHeader& inoPacketHeader,const TCharBuffer::TBuff& inoSubpackets)=0;
	//TODO: Re-implement things in terms of ZeroCopyInputBuffers etc.
	virtual CBasePBResponse::TPointer CreateResponse(const unsigned int innPacketType, TInputStreamList& inaInputStreams)=0;
protected:
	CBasePBResponseFactory() {};
};

};	//namespace syncro
#endif 