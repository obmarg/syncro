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
};

class CBasePBResponseFactory {
public:
	virtual ~CBasePBResponseFactory() {};

	typedef boost::shared_ptr<CBasePBResponseFactory> TPointer;

	virtual CBasePBResponse::TPointer CreateResponse(const pb::PacketHeader& inoPacketHeader,const TCharBuffer::TBuff& inoSubpackets)=0;
protected:
	CBasePBResponseFactory() {};
};

};	//namespace syncro
#endif 