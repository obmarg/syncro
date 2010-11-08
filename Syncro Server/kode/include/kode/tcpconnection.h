#ifndef _KODE_NET_TCPCONNECTION_H_
#define _KODE_NET_TCPCONNECTION_H_

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <string>

namespace kode {
namespace net{

class TCPConnection : boost::noncopyable {
	TCPConnection( std::string hostname, int nPort );
	~TCPConnection();

protected:
	boost::asio::io_service			m_service;
	boost::asio::ip::tcp::socket	m_socket;
};

}	// namespace net
}	// namespace kode

#endif
