#ifndef _SYNCRO_CLIENT_CONNECTION_H_
#define _SYNCRO_CLIENT_CONNECTION_H_

#include <libsyncro/comms.h>
#include <libsyncro/folderlist.h>
#include <kode/tcpconnection.h>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

namespace google {
	namespace protobuf{
		class MessageLite;
		namespace io {
			class ZeroCopyInputStream;
			class ZeroCopyOutputStream;
		}
	}
}


namespace syncro {
namespace client {

class Connection;

class ConnectionDetails {
	friend class Connection;
public:
	ConnectionDetails() : m_port( comms::SERVER_PORT ){};	//TODO: Set some defaults?
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

class UploadFileDetails {
	friend class Connection;
public:
	UploadFileDetails() {};
	UploadFileDetails& SetFolderId(int folderId) { 
		m_folderId = folderId;
		return (*this);
	}
	UploadFileDetails& SetRemotePath(std::string remotePath) { 
		m_remotePath = remotePath;
		return (*this);
	}
	UploadFileDetails& SetLocalPath(std::string localPath) { 
		m_localPath = localPath;
		return (*this);
	}
private:
	int				m_folderId;
	std::string		m_localPath;
	std::string		m_remotePath;
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
	class SendPacket : boost::noncopyable {
	public:
		virtual ~SendPacket() {};
		virtual unsigned int GetSize() const =0 ;
		virtual void Write( 
			google::protobuf::io::ZeroCopyOutputStream& stream 
			) const =0 ;
	};
	typedef boost::shared_ptr<SendPacket> TSendPacketPtr;
	typedef std::vector<TSendPacketPtr> TSendPacketList;
	
public:
	Connection( const ConnectionDetails& details );
	~Connection();
	
	const std::string GetUuid() const {
		return m_uuid;
	}
	const std::string GetServerName() const {
		return m_serverName;
	}

	void GetFolderList(FolderList& list);

	void UploadFile(const UploadFileDetails& details); 
	
	void SendAdminCommand( const std::string& command, const std::string& param );
protected:

	void DoHandshake();

	void SendProtocolBuffer( uint32_t packetType, const google::protobuf::MessageLite& message );
	void SendProtocolBuffer( uint32_t packetType, const TSendPacketList& subpackets );
	TRecvPacketPtr RecvProtocolBuffer(uint32_t expectedPacketType, unsigned int expectedNumSubpackets);
	TRecvPacketPtr RecvProtocolBuffer();

	const ConnectionDetails m_serverDetails;

private:
	std::string m_uuid;
	std::string m_serverName;
};

}	// namespace client
}	// namespace syncro

#endif