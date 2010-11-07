#ifndef _SYNCRO_PB_RESPONSE_FACTORY_H_
#define _SYNCRO_PB_RESPONSE_FACTORY_H_

#include "common.h"
#include "BasePBResponse.h"
#include "FileSendData.h"
#include <boost/scoped_ptr.hpp>
#include "FolderMan.h"
#include "BinaryIncomingData.h"
#include "AdminCommandManager.h"

namespace syncro {

enum eSyncroPBPacketTypes {
	eSyncroPBPacketTypes_BinaryRequest = 1,			// 1
	eSyncroPBPacketTypes_BinaryContinue,			// 2
	eSyncroPBPacketTypes_BinaryResponse,			// 3
	eSyncroPBPacketTypes_HandshakeRequest,			// 4
	eSyncroPBPacketTypes_HandshakeResponse,			// 5
	eSyncroPBPacketTypes_BinaryIncomingRequest,		// 6
	eSyncroPBPacketTypes_BinaryIncomingResponse,	// 7
	eSyncroPBPacketTypes_BinaryIncomingData,		// 8
	eSyncroPBPacketTypes_BinaryIncomingDataAck,		// 9
	eSyncroPBPacketTypes_AdminGenericCommand,		// 10
	eSyncroPBPacketTypes_AdminAck,					// 11
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
	boost::scoped_ptr<CAdminCommandManager> m_pAdminCommandMan;

	bool m_fAuthenticated;
	CAuthToken m_oAuthToken;
};

}; //namespace syncro
#endif 