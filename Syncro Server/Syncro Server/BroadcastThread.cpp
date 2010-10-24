#include "BroadcastThread.h"
#include "BroadcastListener.h"
#include <boost/asio/ip/host_name.hpp>
#include "SyncroDB.h"

namespace syncro {

CBroadcastThread::CBroadcastThread() {
	//TODO: get the hostname from the database or something
	m_sServerName = boost::asio::ip::host_name();
}

CBroadcastThread::~CBroadcastThread() {

}

void
CBroadcastThread::operator()() {
	boost::asio::io_service oIOService;
	CBroadcastListener oListener(oIOService,m_sServerName);
	oIOService.run();
}

};
