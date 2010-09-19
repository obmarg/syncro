#include "BroadcastListener.h"
#include <boost/bind.hpp>

using boost::asio::ip::udp;
using std::string;

const std::string RESPOND_MESSAGE = "SyncroHELLO";

CBroadcastListener::CBroadcastListener(boost::asio::io_service& inoIOService,const std::string &insServerName)
	:m_oSocket( inoIOService, udp::endpoint(udp::v4(), 9995) ),
	 m_sServerName( insServerName ) {
		StartRecieve();
}

CBroadcastListener::~CBroadcastListener() {

}

void
CBroadcastListener::HandleReceive(const boost::system::error_code& error, std::size_t innSize ) {
	BOOL fOK = FALSE;
	if( innSize >= RESPOND_MESSAGE.length() ) {
		fOK = TRUE;
		for(int nChar=0; nChar < RESPOND_MESSAGE.length();nChar++ ) {
			if( RESPOND_MESSAGE[nChar] != m_aRecvBuff[nChar] ) 
				fOK = FALSE;
		}
	}
	if( fOK ) {
		string oResponse = "HELLO: " + m_sServerName;
		m_oSocket.send_to( boost::asio::buffer(oResponse), m_oEndpoint );
	}
}

void 
CBroadcastListener::StartRecieve() {
	m_oSocket.async_receive_from(
        boost::asio::buffer(m_aRecvBuff), m_oEndpoint,
        boost::bind(&CBroadcastListener::HandleReceive, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
}