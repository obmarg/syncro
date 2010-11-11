#ifndef _SYNCRO_CLIENT_CONNECTION_H_
#define _SYNCRO_CLIENT_CONNECTION_H_

#include <kode/tcpconnection.h>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <google/protobuf/message_lite.h>

namespace syncro {
namespace client {

class Connection;

class ConnectionDetails {
	friend class Connection;
public:
	ConnectionDetails();
	ConnectionDetails& SetHostname( const std::string& host ) {
		m_host = host;
		return (*this);
	}
	ConnectionDetails& SetPort( const unsigned int port ) {
		m_port = port;
		return (*this);
	}
	ConnectionDetails& SetUsername( const std::string& username ) {
		m_username = username;
		return (*this);
	}
	ConnectionDetails& SetPassword( const std::string& password ) {
		m_password = password;
		return (*this);
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
protected:
	void DoHandshake();

	void SendProtocolBuffer( const google::protobuf::MessageLite& message );
	void RecvProtocolBuffer( google::protobuf::MessageLite& message );

	//Use this version to skip reading the header and just read a protocol buffer object
	void RecvProtocolBuffer( google::protobuf::MessageLite& message, unsigned int recvSize );

	const ConnectionDetails m_serverDetails;
};

}	// namespace client
}	// namespace syncro

#endif