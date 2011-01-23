/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SYNCRO_CLIENT_CONNECTION_H_
#define _SYNCRO_CLIENT_CONNECTION_H_

#include <libsyncro/comms.h>
#include <libsyncro/folderlist.h>
#include <kode/tcpconnection.h>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

namespace google
{
namespace protobuf
{
class MessageLite;
namespace io
{
class ZeroCopyInputStream;
class ZeroCopyOutputStream;
}
}
}


namespace syncro
{
namespace client
{

class Connection;

class ConnectionDetails
{
	friend class Connection;
	typedef std::vector< uint8_t > HashType;
public:
	ConnectionDetails() : m_port( comms::SERVER_PORT ) {};	//TODO: Set some defaults?
	ConnectionDetails& SetHostname( const std::string& host )
	{
		m_host = host;
		return ( *this );
	}
	ConnectionDetails& SetPort( const unsigned int port )
	{
		m_port = port;
		return ( *this );
	}
	ConnectionDetails& SetUsername( const std::string& username )
	{
		m_username = username;
		return ( *this );
	}
	ConnectionDetails& SetPasswordHash( const HashType& password )
	{
		m_passwordHash = password;
		return ( *this );
	}
	ConnectionDetails& SetPassword( const std::string& password )
	{
		HashPassword( password );
		return ( *this );
	}
private:
	std::string		m_host;
	std::string		m_username;
	HashType		m_passwordHash;

	unsigned int	m_port;
	void HashPassword( const std::string& password );
};

class UploadFileDetails
{
	friend class Connection;
public:
	UploadFileDetails() {};
	UploadFileDetails& SetFolderId( int folderId )
	{
		m_folderId = folderId;
		return ( *this );
	}
	UploadFileDetails& SetRemotePath( std::string remotePath )
	{
		m_remotePath = remotePath;
		return ( *this );
	}
	UploadFileDetails& SetLocalPath( std::string localPath )
	{
		m_localPath = localPath;
		return ( *this );
	}
	UploadFileDetails& SetOneShot( bool oneShot )
	{
		m_oneShot = oneShot;
		return ( *this );
	}
private:
	int				m_folderId;
	std::string		m_localPath;
	std::string		m_remotePath;
	bool			m_oneShot;
};

typedef boost::shared_ptr< google::protobuf::io::ZeroCopyInputStream > TRecvStream;
typedef boost::shared_ptr< google::protobuf::io::ZeroCopyOutputStream > TWriteStream;

class Connection : private kode::net::TCPConnection
{
public:
	class RecvPacket : boost::noncopyable
	{
	public:
		virtual unsigned int GetNumSubpackets() = 0;
		virtual TRecvStream ReadSubpacket( unsigned int num ) = 0;
		virtual unsigned int GetPacketType() = 0;
	};
	typedef boost::shared_ptr<RecvPacket> TRecvPacketPtr;
	class SendPacket : boost::noncopyable
	{
	public:
		virtual ~SendPacket() {};
		virtual unsigned int GetSize() const = 0 ;
		virtual void Write(
		    google::protobuf::io::ZeroCopyOutputStream& stream
		) const = 0 ;
	};
	typedef boost::shared_ptr<SendPacket> TSendPacketPtr;
	typedef std::vector<TSendPacketPtr> TSendPacketList;

	typedef std::map< std::string, std::string > StringMap;

public:
	Connection( const ConnectionDetails& details );
	~Connection();

	const std::string GetUuid() const
	{
		return m_uuid;
	}
	const std::string GetServerName() const
	{
		return m_serverName;
	}

	void GetFolderList( FolderList& list );

	void UploadFile( const UploadFileDetails& details );

	void SendAdminCommand( const std::string& command, const StringMap& params );
protected:

	void DoHandshake();

	void SendProtocolBuffer( uint32_t packetType, const google::protobuf::MessageLite& message );
	void SendProtocolBuffer( uint32_t packetType, const TSendPacketList& subpackets );
	TRecvPacketPtr RecvProtocolBuffer( uint32_t expectedPacketType, unsigned int expectedNumSubpackets );
	TRecvPacketPtr RecvProtocolBuffer();

	const ConnectionDetails m_serverDetails;

private:
	std::string m_uuid;
	std::string m_serverName;
};

}	// namespace client
}	// namespace syncro

#endif
