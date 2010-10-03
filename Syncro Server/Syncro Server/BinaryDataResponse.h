#ifndef _BINARY_DATA_RESPONSE_H_
#define _BINARY_DATA_RESPONSE_H_

#include "common.h"
#include "VectorPBResponse.h"
#include "SyncroPBResponseFactory.h"
#include "FileSendData.h"

namespace syncro {

class CBinaryDataResponse : public CVectorPBResponse {
public:
	CBinaryDataResponse(CFileSendData& inoFileData);
	virtual ~CBinaryDataResponse();

	virtual unsigned int GetPacketType() { 
		return eSyncroPBPacketTypes_BinaryResponse;
	};
};

};		//namespace syncro

#endif