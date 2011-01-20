#ifndef _BROADCAST_THREAD_H_
#define _BROADCAST_THREAD_H_

#include <string>

namespace syncro
{

class CBroadcastThread
{
public:
	CBroadcastThread();
	~CBroadcastThread();

	void operator()();
private:
	std::string m_sServerName;
};

};	//namespace syncro

#endif