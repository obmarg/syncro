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

#include "connection.h"
#include "comms.h"
#include "packet_types.h"
#include "protocol_buffers/handshake.pb.h"
#include "protocol_buffers/header.pb.h"
#include "protocol_buffers/folders.pb.h"
#include "protocol_buffers/admin.pb.h"
#include "protocol_buffers/binarydata.pb.h"
#include <kode/base64.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <boost/shared_array.hpp>
#include <boost/scoped_array.hpp>
#include <boost/filesystem.hpp>
#include <stdexcept>
#include <fstream>


namespace syncro
{
namespace client
{

typedef boost::shared_array<unsigned char> TBufferPtr;
struct sBuffer
{
	unsigned int size;
	TBufferPtr buffer;
};
typedef std::vector<sBuffer> TBufferList;

class RecvPacketImp : public Connection::RecvPacket
{
public:
	RecvPacketImp( unsigned int packetType, unsigned int numSubpackets ) :
		m_packetType( packetType ),
		m_buffers( numSubpackets )
	{
	}
	virtual unsigned int GetNumSubpackets()
	{
		return m_buffers.size();
	}
	virtual TRecvStream ReadSubpacket( unsigned int num )
	{
		return TRecvStream( new google::protobuf::io::ArrayInputStream( m_buffers[ num ].buffer.get(), m_buffers[ num ].size ) );
	}
	virtual unsigned int GetPacketType()
	{
		return m_packetType;
	}

	TWriteStream WriteSubpacket( unsigned int num, unsigned int size )
	{
		m_buffers[num].buffer.reset( new unsigned char[size] );
		m_buffers[num].size = size;
		TWriteStream rv( new google::protobuf::io::ArrayOutputStream( m_buffers[num].buffer.get(), size ) );
		return rv;
	}
private:
	unsigned int m_packetType;
	TBufferList m_buffers;
};

const size_t RECV_BUFFER_SIZE = 512;
const size_t SEND_BUFFER_SIZE = 1024 * 1024;

class ProtocolBufferPacket : public Connection::SendPacket
{
public:
	virtual ~ProtocolBufferPacket() {};
	virtual unsigned int GetSize() const
	{
		return m_message.ByteSize();
	};
	virtual void Write(
	    google::protobuf::io::ZeroCopyOutputStream& stream
	) const
	{
		m_message.SerializeToZeroCopyStream( &stream );
	};
	static Connection::TSendPacketPtr Create( const google::protobuf::MessageLite& message )
	{
		return Connection::TSendPacketPtr(
		           new ProtocolBufferPacket( message )
		       );
	}
private:
	ProtocolBufferPacket( const google::protobuf::MessageLite& message )
		: m_message( message )
	{};

	const google::protobuf::MessageLite& m_message;
};

class FileStreamPacket : public Connection::SendPacket
{
public:
	virtual ~FileStreamPacket() {};
	virtual unsigned int GetSize() const
	{
		return m_size;
	}
	virtual void Write(
	    google::protobuf::io::ZeroCopyOutputStream& stream
	) const
	{
		void* buffer;
		int size;
		unsigned int sizeWritten = 0;
		while( stream.Next( &buffer, &size ) && !m_file.eof() )
		{
			m_file.read(( char* )buffer, size );
			std::streamsize sizeRead = m_file.gcount();
			if(( sizeRead != size ) && ( m_file.eof() || m_file.fail() ) )
			{
				throw std::runtime_error(
				    "FileStreamPacket::Write failed - unexpected EOF in file?"
				);
			}
			sizeWritten += size;
		}
		if( sizeWritten != m_size )
			throw std::runtime_error(
			    "FileStreamPacket::Write failed - not enough data written to buffer"
			);
	}
	static Connection::TSendPacketPtr Create( std::ifstream& file, unsigned int size )
	{
		return Connection::TSendPacketPtr(
		           new FileStreamPacket( file, size )
		       );
	}
private:
	FileStreamPacket( std::ifstream& file, unsigned int size ) :
		m_file( file ),
		m_size( size )
	{
	};
	mutable std::ifstream& m_file;
	unsigned int m_size;
};

void ConnectionDetails::HashPassword( const std::string& password )
{
	m_passwordHash.resize( CryptoPP::SHA::DIGESTSIZE );
	CryptoPP::SHA().CalculateDigest(
	    &m_passwordHash[0],
	    reinterpret_cast< const unsigned char* >( password.c_str() ),
	    password.length()
	);
}

Connection::Connection( const ConnectionDetails& details ) :
	TCPConnection( details.m_host, details.m_port ),
	m_serverDetails( details )
{
	DoHandshake();
}

Connection::~Connection()
{

}

void
Connection::SendProtocolBuffer( uint32_t packetType, const google::protobuf::MessageLite& message )
{
	pb::PacketHeader pbHeader;
	pbHeader.set_packet_type( packetType );
	pbHeader.add_subpacket_sizes( message.ByteSize() );

	std::vector<unsigned char> buffer(
	    message.ByteSize() + comms::PacketHeader::BYTE_SIZE + pbHeader.ByteSize()
	);

	comms::PacketHeader()
	.SetFirstByte( comms::PB_REQUEST_FIRST_BYTE )
	.SetPacketSize( pbHeader.ByteSize() )
	.Write( buffer );

	google::protobuf::io::ArrayOutputStream oStream1(
	    &buffer[ comms::PacketHeader::BYTE_SIZE ],
	    pbHeader.ByteSize(),
	    pbHeader.ByteSize()
	);

	pbHeader.SerializeToZeroCopyStream( &oStream1 );

	google::protobuf::io::ArrayOutputStream oStream2(
	    &buffer[ comms::PacketHeader::BYTE_SIZE + pbHeader.ByteSize()],
	    message.ByteSize(),
	    message.ByteSize()
	);

	message.SerializeToZeroCopyStream( &oStream2 );

	Send( buffer );
}

void
Connection::SendProtocolBuffer( uint32_t packetType, const Connection::TSendPacketList& subpackets )
{
	pb::PacketHeader pbHeader;
	pbHeader.set_packet_type( packetType );
	unsigned int subPacketSizes = 0;
	BOOST_FOREACH( const TSendPacketPtr & subpacket, subpackets )
	{
		pbHeader.add_subpacket_sizes( subpacket->GetSize() );
		subPacketSizes += subpacket->GetSize();
	}
	unsigned int sizeNeeded(
	    comms::PacketHeader::BYTE_SIZE
	    + pbHeader.ByteSize()
	    + subPacketSizes
	);

	std::vector<unsigned char> buffer( sizeNeeded );

	comms::PacketHeader()
	.SetFirstByte( comms::PB_REQUEST_FIRST_BYTE )
	.SetPacketSize( pbHeader.ByteSize() )
	.Write( buffer );

	google::protobuf::io::ArrayOutputStream oStream1(
	    &buffer[ comms::PacketHeader::BYTE_SIZE ],
	    pbHeader.ByteSize(),
	    pbHeader.ByteSize()
	);

	pbHeader.SerializeToZeroCopyStream( &oStream1 );

	unsigned int sizeSoFar = comms::PacketHeader::BYTE_SIZE + pbHeader.ByteSize();
	BOOST_FOREACH( const TSendPacketPtr & subpacket, subpackets )
	{
		google::protobuf::io::ArrayOutputStream oStream2(
		    &buffer[ sizeSoFar ],
		    subpacket->GetSize(),
		    subpacket->GetSize()
		);
		sizeSoFar += subpacket->GetSize();
		subpacket->Write( oStream2 );
	}

	Send( buffer );
}

Connection::TRecvPacketPtr
Connection::RecvProtocolBuffer( uint32_t expectedPacketType, unsigned int expectedNumSubpackets )
{
	TRecvPacketPtr rv = RecvProtocolBuffer();
	comms::packet_types::ePBPacketTypes expectedType =
	    static_cast< comms::packet_types::ePBPacketTypes >(
	        expectedPacketType
	    );
	comms::packet_types::ePBPacketTypes type =
	    static_cast< comms::packet_types::ePBPacketTypes >(
	        rv->GetPacketType()
	    );
	if( type != expectedType )
	{
		std::stringstream error;
		error << "Unexpected packet type in RecvProtocolBuffer.\n" <<
		      "Expected: " <<
		      syncro::comms::packet_types::Str( expectedType ) <<
		      " Actual: " <<
		      syncro::comms::packet_types::Str( type ) <<
		      "\n";
		throw std::runtime_error( error.str().c_str() );
	}
	if( rv->GetNumSubpackets() != expectedNumSubpackets )
	{
		std::stringstream error;
		error << "Unexpected number of subpackets in RecvProtocolBuffer.\n" <<
		      "Expected: " << expectedNumSubpackets <<
		      " Actual: " << rv->GetNumSubpackets() << "\n" <<
		      "Packet Type: " << comms::packet_types::Str( expectedType ) <<
		      "\n";
		throw std::runtime_error( error.str().c_str() );
	}
	return rv;
}

Connection::TRecvPacketPtr
Connection::RecvProtocolBuffer( )
{
	std::vector<unsigned char> aBuffer( RECV_BUFFER_SIZE );

	Recv( aBuffer, comms::PacketHeader::BYTE_SIZE );
	comms::PacketHeader oFirstHeader;
	oFirstHeader.Read( &aBuffer[0] );

	if( oFirstHeader.FirstByte() != comms::PB_RESPONSE_FIRST_BYTE )
	{
		throw std::runtime_error( "Invalid first byte in syncro::client::connection::RecvProtocolBuffer" );
	}
	unsigned int pbPacketHeaderSize = oFirstHeader.PacketSize();
	//TODO: FInish pb headering this code

	std::vector<unsigned char> pbHeaderBuffer( pbPacketHeaderSize );
	Recv( pbHeaderBuffer, pbPacketHeaderSize );

	pb::PacketHeader pbHeader;
	pbHeader.ParseFromArray( &pbHeaderBuffer[0], pbPacketHeaderSize );

	RecvPacketImp* packet = new RecvPacketImp( pbHeader.packet_type(), pbHeader.subpacket_sizes_size() );
	TRecvPacketPtr rv( packet );

	for( int packetNum = 0; packetNum < pbHeader.subpacket_sizes_size( ); packetNum++ )
	{
		unsigned int subpacketSize = pbHeader.subpacket_sizes( packetNum );
		TWriteStream stream = packet->WriteSubpacket( packetNum, subpacketSize );
		unsigned int done = 0;
		void* currentBlock = NULL;
		int currentBlockSize = 0;
		while(( done < subpacketSize ) && stream->Next( &currentBlock, &currentBlockSize ) )
		{
			RecvByVal( reinterpret_cast<char*>( currentBlock ), currentBlockSize );
			done += currentBlockSize;
		}
		if( done < subpacketSize )
			throw std::runtime_error( "stream->Next failed in syncro::client::Connection::RecvProtocolBuffer" );
	}
	return rv;
}

void Connection::DoHandshake()
{
	pb::HandshakeRequest oRequest;

	if( !m_serverDetails.m_passwordHash.empty() )
	{
		std::vector< unsigned char > salt( CryptoPP::SHA::DIGESTSIZE );
		pb::SaltRequest saltRequest;
		saltRequest.set_size( CryptoPP::SHA::DIGESTSIZE );
		SendProtocolBuffer( comms::packet_types::SaltRequest, saltRequest );
		TRecvPacketPtr saltResponsePacket = RecvProtocolBuffer(
		                                        comms::packet_types::SaltResponse, 1
		                                    );
		pb::SaltResponse saltResponse;
		saltResponse.ParseFromZeroCopyStream( saltResponsePacket->ReadSubpacket( 0 ).get() );
		if( !saltResponse.has_salt() )
			throw std::runtime_error( "Salt response does not contain a salt" );
		kode::base64::Decode( saltResponse.salt(), salt );

		std::vector< unsigned char > sendPassword( CryptoPP::SHA::DIGESTSIZE );

		CryptoPP::HMAC< CryptoPP::SHA >( &salt[0], salt.size() )
		.CalculateDigest(
		    &sendPassword[0],
		    &m_serverDetails.m_passwordHash[0],
		    m_serverDetails.m_passwordHash.size()
		);

		oRequest.set_password(
		    kode::base64::Encode( &sendPassword[0], sendPassword.size() )
		);
	}

	oRequest.set_client_ver_major( 0 );
	oRequest.set_client_ver_major( 6 );
	oRequest.set_magic( comms::HANDSHAKE_REQUEST_MAGIC );
	if( !m_serverDetails.m_username.empty() )
	{
		oRequest.set_username( m_serverDetails.m_username );
	}

	SendProtocolBuffer( comms::packet_types::HandshakeRequest , oRequest );

	TRecvPacketPtr responsePacket = 
		RecvProtocolBuffer( comms::packet_types::HandshakeResponse, 1 );

	pb::HandshakeResponse response;
	response.ParsePartialFromZeroCopyStream( responsePacket->ReadSubpacket( 0 ).get() );
	if( !response.has_magic() || response.magic().compare( comms::HANDSHAKE_RESPONSE_MAGIC ) != 0 )
	{
		throw std::runtime_error( "Handshake response magic incorrect" );
	}
	if( response.has_uuid() )
	{
		m_uuid = response.uuid();
	}
	//TODO: Check for authentication success/failure, and save the uuid?
}

void Connection::GetFolderList( FolderList& list )
{
	pb::FolderListRequest request;
	request.set_search_string( "blah" );

	SendProtocolBuffer( comms::packet_types::FolderListRequest, request );

	TRecvPacketPtr responsePacket = RecvProtocolBuffer(
	                                    comms::packet_types::FolderListResponse, 1
	                                );
	pb::FolderList response;
	response.ParseFromZeroCopyStream( responsePacket->ReadSubpacket( 0 ).get() );
	list.resize( response.folders_size() );
	for( int folderNum = 0; folderNum < response.folders_size(); folderNum++ )
	{
		const pb::FolderInfo& folder = response.folders( folderNum );
		if( folder.has_folder_id() )
			list[ folderNum ].Id = folder.folder_id();
		if( folder.has_folder_name() )
			list[ folderNum ].Name = folder.folder_name();
		if( folder.has_folder_path() )
			list[ folderNum ].Path = folder.folder_path();
		//TODO: Fill these 2 in at some point...
		if( folder.has_can_read() ) {};
		if( folder.has_can_write() ) {};
	}
}

//TODO: Probably want to replace this with a better implementation at some
//		point
static void ProcessFolder( 
	const pb::FolderContents& contents, 
	FileList& fileList,
	const std::string& pathSoFar
	)
{
	fileList.reserve( fileList.size() + contents.files_size() );
	for( int fileNum = 0; fileNum < contents.files_size(); fileNum++ )
	{
		const pb::FileInfo &file = contents.files(fileNum);
		fileList.push_back(
			FileInfo( pathSoFar + file.name(), file.size() )
			);
	}
	for( 
		int folderNum = 0; 
		folderNum < contents.subfolders_size(); 
		folderNum++ 
		)
	{
		const pb::FolderContents& subfolder = 
			contents.subfolders( folderNum );

		std::string newPath = pathSoFar + subfolder.name() + "/";
		ProcessFolder( subfolder, fileList, newPath	);
	}
}

void Connection::GetFolderContents( int folderId, FileList& contents )
{
	pb::FolderContentsRequest request;
	request.set_folder_id( folderId );

	SendProtocolBuffer( comms::packet_types::FolderContentsRequest, request );

	TRecvPacketPtr responsePacket = 
		RecvProtocolBuffer( comms::packet_types::FolderContentsResponse, 1 );

	pb::FolderContents response;
	response.ParseFromZeroCopyStream( 
		responsePacket->ReadSubpacket( 0 ).get() 
		);

	ProcessFolder( response, contents, "" );
}

void Connection::SendAdminCommand( const std::string& command, const StringMap& params )
{
	pb::GenericAdminCommand request;
	request.set_command( command );

	StringMap::const_iterator
	it = params.begin(),
	itEnd = params.end();
	for( ; it != itEnd; ++it )
	{
		syncro::pb::AdminParameter* param = request.add_params();
		param->set_name( it->first );
		param->set_string_value( it->second );
	}

	SendProtocolBuffer( comms::packet_types::AdminGenericCommand, request );

	TRecvPacketPtr responsePacket = RecvProtocolBuffer(
	                                    comms::packet_types::AdminAck, 1
	                                );
	pb::AdminAck ack;
	ack.ParseFromZeroCopyStream( responsePacket->ReadSubpacket( 0 ).get() );
	if( ack.has_ok() )
	{
		if( !ack.ok() )
		{
			//TODO: Add better error reporting etc.  maybe by return value rather than exception?
			throw std::runtime_error( "Admin command failed" );
		}
	}
}

void Connection::UploadFile( const UploadFileDetails& details )
{
	pb::BinaryDataRequest request;
	request.set_file_name( details.m_remotePath );
	request.set_file_size(
	    static_cast<google::protobuf::int32>(
	        boost::filesystem::file_size( details.m_localPath )
	    )
	);
	request.set_folder_id( details.m_folderId );
	request.set_direction(
	    pb::BinaryDataRequest_TransferDirection_Upload
	);
	if( details.m_oneShot )
	{
		request.set_one_shot( details.m_oneShot );
	}
	pb::BinaryIncomingResponse initialResponse;
	SendProtocolBuffer( comms::packet_types::BinaryIncomingRequest, request );

	{
		TRecvPacketPtr responsePacket = RecvProtocolBuffer(
		                                    comms::packet_types::BinaryIncomingResponse, 1
		                                );
		initialResponse.ParseFromZeroCopyStream(
		    responsePacket->ReadSubpacket( 0 ).get()
		);
	}

	if( !initialResponse.has_accepted() )
		throw std::runtime_error( "Initial response does not contain an accepted value" );
	if( initialResponse.accepted() )
	{
		int sendBufferSize = SEND_BUFFER_SIZE;
		if( initialResponse.has_max_packet_size() )
			if( sendBufferSize >  initialResponse.max_packet_size() )
				sendBufferSize = initialResponse.max_packet_size();

		bool error = false;
		std::ifstream file(
		    details.m_localPath.c_str(),
		    std::ios::in | std::ios::binary
		);
		bool start = true;
		file.seekg( 0, std::ios::end );
		std::streamsize totalSize = file.tellg();
		file.seekg( 0, std::ios::beg );
		bool done = false;
		while( !done && !error && !file.fail() )
		{
			pb::BinaryPacketHeader header;
			if( start )
			{
				header.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_START );
				start = false;
			}
			else
				header.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_MIDDLE );
			unsigned int sizeToRead = sendBufferSize;
			unsigned int currPos = static_cast<unsigned int>( file.tellg() );
			if(( currPos + sizeToRead ) > totalSize )
			{
				header.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_END );
				sizeToRead = static_cast<unsigned int>( totalSize ) - currPos;
				done = true;
			}
			TSendPacketList subpackets;
			subpackets.push_back( ProtocolBufferPacket::Create( header ) );
			subpackets.push_back( FileStreamPacket::Create( file, sizeToRead ) );
			SendProtocolBuffer( comms::packet_types::BinaryIncomingData, subpackets );
			TRecvPacketPtr responsePacket =
			    RecvProtocolBuffer( comms::packet_types::BinaryIncomingDataAck, 1 );
			pb::BinaryIncomingAck ack;
			ack.ParseFromZeroCopyStream( responsePacket->ReadSubpacket( 0 ).get() );
			if( !ack.has_ok() )
				throw std::runtime_error(
				    "FileUpload failed - no ok returned in ack"
				);
			if( !ack.ok() )
				throw std::runtime_error(
				    "FileUpload failed - server returned error"
				);
		}
	}
	else
		throw std::runtime_error( "File was rejected by server" );
}


}
}
