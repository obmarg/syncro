#ifndef _SYNCRO_CLIENT_CONNECTION_H_
#define _SYNCRO_CLIENT_CONNECTION_H_

#include <kode/tcpconnection.h>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

namespace syncro {
namespace client {

class Connection;

class ConnectionDetails {
public:
	ConnectionDetails();
	ConnectionDetails& SetHostname( const std::string& host ) {
		m_host = host;
	}
	ConnectionDetails& SetPort( const unsigned int port ) {
		m_port = port;
	}
	ConnectionDetails& SetUsername( const std::string& username ) {
		m_username = username;
	}
	ConnectionDetails& SetPassword( const std::string& password ) {
		m_password = password;
	}
private:
	std::string		m_host;
	std::string		m_username;
	std::string		m_password;
	unsigned int	m_port;
};

class Connection : private kode::net::TCPConnection {
public:
	Connection( const ConnectionDetails& details );
	~Connection();
private:
	
};

}	// namespace client
}	// namespace syncro

#endif