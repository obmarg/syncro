#ifndef _SYNCRO_CLIENT_CONNECTION_H_
#define _SYNCRO_CLIENT_CONNECTION_H_

#include <kode/tcpconnection.h>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/io/zero_copy_stream.h>

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

typedef boost::shared_ptr< google::protobuf::io::ZeroCopyInputStream > TRecvStream;
typedef boost::shared_ptr< google::protobuf::io::ZeroCopyOutputStream > TWriteStream;

class Connection : private kode::net::TCPConnection {
public:
	class RecvPacket : boost::noncopyable {
	public:
		virtual unsigned int GetNumSubpackets()=0;
		virtual TRecvStream ReadSubpacket(unsigned int num)=0;
		virtual unsigned int GetPacketType()=0;
	};
	typedef boost::shared_ptr<RecvPacket> TRecvPacketPtr;
	
public:
	Connection( const ConnectionDetails& details );
	~Connection();
	
	
protected:

	void DoHandshake();

	void SendProtocolBuffer( uint32_t packetType, const google::protobuf::MessageLite& message );
	TRecvPacketPtr RecvProtocolBuffer();

	const ConnectionDetails m_serverDetails;
};

}	// namespace client
}	// namespace syncro

#endif