#include "BroadcastThread.h"
#include "BroadcastListener.h"

CBroadcastThread::CBroadcastThread() {

}

CBroadcastThread::~CBroadcastThread() {

}

void
CBroadcastThread::operator()() {
	boost::asio::io_service oIOService;
	CBroadcastListener oListener(oIOService,"Server");
	while(true) {
		oIOService.run();
		//TODO: sleep the thread or some shite here?
	}
}