#ifndef _SYNCRO_PB_RESPONSE_FACTORY_H_
#define _SYNCRO_PB_RESPONSE_FACTORY_H_

#include "common.h"
#include "BasePBResponse.h"
#include "FileSendData.h"
#include <boost/scoped_ptr.hpp>

namespace syncro {

enum eSyncroPBPacketTypes {
	eSyncroPBPacketTypes_BinaryRequest = 1,
	eSyncroPBPacketTypes_BinaryContinue,
	eSyncroPBPacketTypes_BinaryResponse,
};

struct sSubpackets {
	typedef std::vector<unsigned int> TSizeList;
	unsigned int Count;
	TSizeList Sizes;
	const TCharBuffer::TBuff& Buffer;
};

class CSyncroPBResponseFactory : public CBasePBResponseFactory {
public:
	CSyncroPBResponseFactory(const CSyncroPBResponseFactory& inoOther) {
		//TODO: Implement copying of stuff
	}
	virtual ~CSyncroPBResponseFactory() {};

	static CBasePBResponseFactory::TPointer Create() {
		return CBasePBResponseFactory::TPointer( new CSyncroPBResponseFactory() );
	};

	virtual CBasePBResponse::TPointer CreateResponse(const pb::PacketHeader& inoPacketHeader,const TCharBuffer::TBuff& inoSubpackets);
private:
	CSyncroPBResponseFactory() {};
	typedef boost::scoped_ptr<CFileSendData> TFileSendDataPtr;
	TFileSendDataPtr m_pCurrentSendData;
};

}; //namespace syncro
#endif 