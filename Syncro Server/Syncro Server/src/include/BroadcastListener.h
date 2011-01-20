#ifndef _BROADCAST_LISTENER_H_
#define _BROADCAST_LISTENER_H_

#include <boost/asio.hpp>

namespace syncro
{

class CBroadcastListener
{
public:
	CBroadcastListener( boost::asio::io_service& inoIOService, const std::string& insServerName );
	~CBroadcastListener();

private:
	boost::asio::ip::udp::socket m_oSocket;
	boost::asio::ip::udp::endpoint m_oEndpoint;
	boost::array<char, 20> m_aRecvBuff;

	const std::string m_sServerName;

	void HandleReceive( const boost::system::error_code& error, std::size_t );
	void StartRecieve();
};

}; //namespace syncro

#endif