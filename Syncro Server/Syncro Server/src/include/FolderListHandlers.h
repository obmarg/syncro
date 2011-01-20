#ifndef __FOLDER_LIST_HANDLERS_H_
#define __FOLDER_LIST_HANDLERS_H_

#include "common.h"
#include "BasePBResponse.h"

namespace syncro
{

class FolderListRequestHandler
{
public:
	FolderListRequestHandler( TInputStreamList& inaInputStreams );

	CBasePBResponse::TPointer GetResponse();
};

}	// namespace syncro

#endif
