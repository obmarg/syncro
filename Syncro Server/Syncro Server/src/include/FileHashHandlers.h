#ifndef SYNCRO_FILE_HASH_HANDLERS_H_
#define SYNCRO_FILE_HASH_HANDLERS_H_

#include "common.h"
#include "BasePBResponse.h"

namespace syncro {

class CFolderMan;

namespace pbHandlers {

class FileHashRequest {
public:
	FileHashRequest(
		TInputStreamList& inaInputStreams,
		CFolderMan& folderMan
		);
	CBasePBResponse::TPointer GetResponse();
private:
	bool m_ok;
};

}	// namespace pbHandlers
}	// namespace syncro

#endif