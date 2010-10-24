#include "BroadcastThread.h"
#include "BroadcastListener.h"

namespace syncro {

CBroadcastThread::CBroadcastThread() {

}

CBroadcastThread::~CBroadcastThread() {

}

void
CBroadcastThread::operator()() {
	boost::asio::io_service oIOService;
	CBroadcastListener oListener(oIOService,"Server");
	oIOService.run();
}

};
