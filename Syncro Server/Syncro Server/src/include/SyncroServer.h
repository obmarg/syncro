#ifndef _SYNCRO_SERVER_H_
#define _SYNCRO_SERVER_H_

#include "FolderMan.h"
#include <boost/thread.hpp>

namespace syncro
{

class CSyncroServer
{
public:
	CSyncroServer();
	~CSyncroServer();

	bool Run();
private:
	boost::thread m_oBroadcastThread;
};

};		//namespace syncro

#endif