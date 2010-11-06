#ifndef _SYNCRO_PB_RESPONSE_FACTORY_H_
#define _SYNCRO_PB_RESPONSE_FACTORY_H_

#include "common.h"
#include "BasePBResponse.h"
#include "FileSendData.h"
#include <boost/scoped_ptr.hpp>
#include "FolderMan.h"
#include "BinaryIncomingData.h"

namespace syncro {

enum eSyncroPBPacketTypes {
	eSyncroPBPacketTypes_BinaryRequest = 1,
	eSyncroPBPacketTypes_BinaryContinue,
	eSyncroPBPacketTypes_BinaryResponse,
	eSyncroPBPacketTypes_HandshakeRequest,
	eSyncroPBPacketTypes_HandshakeResponse,
	eSyncroPBPacketTypes_BinaryIncomingRequest,
	eSyncroPBPacketTypes_BinaryIncomingResponse,
	eSyncroPBPacketTypes_BinaryIncomingData,
	eSyncroPBPacketTypes_BinaryIncomingDataAck,
};

struct sSubpackets {
	typedef std::vector<unsigned int> TSizeList;
	unsigned int Count;
	TSizeList Sizes;
	const TCharBuffer::TBuff& Buffer;
};

class CSyncroPBResponseFactory : public CBasePBResponseFactory {
public:
	CSyncroPBResponseFactory(const CSyncroPBResponseFactory& inoOther){
		//TODO: Implement copying of stuff (or make uncopyable?)
	}
	virtual ~CSyncroPBResponseFactory() {};

	static CBasePBResponseFactory::TPointer Create() {
		return CBasePBResponseFactory::TPointer( new CSyncroPBResponseFactory() );
	};

	virtual CBasePBResponse::TPointer CreateResponse(const unsigned int innPacketType,TInputStreamList& inaInputStreams);
private:
	CSyncroPBResponseFactory();
	typedef boost::scoped_ptr<CFileSendData> TFileSendDataPtr;
	TFileSendDataPtr m_pCurrentSendData;
	boost::scoped_ptr<CFolderMan> m_pFolderMan;
	typedef boost::scoped_ptr<CBinaryIncomingData> TFileRecvDataPtr;
	TFileRecvDataPtr m_pCurrentRecvData;
	bool m_fAuthenticated;
};

}; //namespace syncro
#endif 