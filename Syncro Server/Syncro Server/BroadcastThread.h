#ifndef _BROADCAST_THREAD_H_
#define _BROADCAST_THREAD_H_

class CBroadcastThread {
public:
	CBroadcastThread();
	~CBroadcastThread();

	void operator()();
};

#endif