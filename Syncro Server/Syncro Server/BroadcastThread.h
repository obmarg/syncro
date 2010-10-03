#ifndef _BROADCAST_THREAD_H_
#define _BROADCAST_THREAD_H_

namespace syncro {

class CBroadcastThread {
public:
	CBroadcastThread();
	~CBroadcastThread();

	void operator()();
};

};	//namespace syncro

#endif