#ifndef SYNCRO_FILE_HASH_HANDLERS_H_
#define SYNCRO_FILE_HASH_HANDLERS_H_

#include "common.h"
#include "BasePBResponse.h"
#include <boost/function.hpp>

namespace syncro
{

class CFolderMan;

namespace pbHandlers
{

class FileHashRequest
{
public:
	typedef boost::function< void (const std::string&, int64_t) >
		HashOkCallback;
public:
	FileHashRequest(
	    TInputStreamList& inaInputStreams,
	    CFolderMan& folderMan,
		HashOkCallback callback=HashOkCallback()
	);
	CBasePBResponse::TPointer GetResponse();
private:
	bool m_ok;
};

}	// namespace pbHandlers
}	// namespace syncro

#endif