#include "connection.h"
#include "comms.h"
#include "packet_types.h"
#include "protocol_buffers/handshake.pb.h"
#include "protocol_buffers/header.pb.h"
#include "protocol_buffers/folders.pb.h"
#include "protocol_buffers/admin.pb.h"
#include "protocol_buffers/binarydata.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <boost/shared_array.hpp>
#include <boost/scoped_array.hpp>
#include <boost/filesystem.hpp>
#include <stdexcept>
#include <fstream>


namespace syncro {
namespace client {

typedef boost::shared_array<unsigned char> TBufferPtr;
struct sBuffer {
	unsigned int size;
	TBufferPtr buffer;
};
typedef std::vector<sBuffer> TBufferList;

class RecvPacketImp : public Connection::RecvPacket {
public:
	RecvPacketImp(unsigned int packetType, unsigned int numSubpackets) :
	  m_packetType( packetType ),
	  m_buffers( numSubpackets )
	{
	}
	virtual unsigned int GetNumSubpackets()
	{
		return m_buffers.size();
	}
	virtual TRecvStream ReadSubpacket(unsigned int num)
	{
		return TRecvStream ( new google::protobuf::io::ArrayInputStream( m_buffers[ num ].buffer.get(), m_buffers[ num ].size ) );
	}
	virtual unsigned int GetPacketType() {
		return m_packetType;
	}

	TWriteStream WriteSubpacket(unsigned int num, unsigned int size) {
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
	static Connection::TSendPacketPtr Create(const google::protobuf::MessageLite& message)
	{
		return Connection::TSendPacketPtr(
			new ProtocolBufferPacket( message )
			);
	}
private:
	ProtocolBufferPacket(const google::protobuf::MessageLite& message)
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
		void *buffer;
		int size;
		unsigned int sizeWritten = 0;
		while( stream.Next( &buffer, &size ) && !m_file.eof() )
		{
			m_file.read( (char*)buffer, size );
			if( m_file.gcount() != size )
				throw std::runtime_error( 
					"FileStreamPacket::Write failed - unexpected EOF?" 
					);
			sizeWritten += size;
		}
		if( sizeWritten != m_size )
			throw std::runtime_error(
				"FileStreamPacket::Write failed - not enough data written"
				);
	}
	static Connection::TSendPacketPtr Create(std::ifstream& file,unsigned int size)
	{
		return Connection::TSendPacketPtr(
			new FileStreamPacket( file,size )
			);
	}
private:
	FileStreamPacket( std::ifstream& file,unsigned int size ) :
		m_file( file ),
		m_size( size )
	{
	};
	mutable std::ifstream& m_file;
	unsigned int m_size;
};

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
Connection::SendProtocolBuffer( uint32_t packetType, const google::protobuf::MessageLite& message ) {
	pb::PacketHeader pbHeader;
	pbHeader.set_packet_type(packetType);
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
		&buffer[ comms::PacketHeader::BYTE_SIZE + pbHeader.ByteSize() ],
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
	pbHeader.set_packet_type(packetType);
	unsigned int subPacketSizes = 0;
	BOOST_FOREACH( const TSendPacketPtr& subpacket, subpackets ) {
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
	BOOST_FOREACH( const TSendPacketPtr& subpacket, subpackets ) {
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
Connection::RecvProtocolBuffer(uint32_t expectedPacketType, unsigned int expectedNumSubpackets)
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
Connection::RecvProtocolBuffer(  ) {
	std::vector<unsigned char> aBuffer( RECV_BUFFER_SIZE );
	
	Recv( aBuffer, comms::PacketHeader::BYTE_SIZE );
	comms::PacketHeader oFirstHeader;
	oFirstHeader.Read( &aBuffer[0] );

	if( oFirstHeader.FirstByte() != comms::PB_RESPONSE_FIRST_BYTE ) {
		throw std::runtime_error( "Invalid first byte in syncro::client::connection::RecvProtocolBuffer");
	}
	unsigned int pbPacketHeaderSize = oFirstHeader.PacketSize();
	//TODO: FInish pb headering this code
	
	std::vector<unsigned char> pbHeaderBuffer( pbPacketHeaderSize );
	Recv( pbHeaderBuffer, pbPacketHeaderSize );

	pb::PacketHeader pbHeader;
	pbHeader.ParseFromArray( &pbHeaderBuffer[0], pbPacketHeaderSize );
	
	RecvPacketImp* packet = new RecvPacketImp( pbHeader.packet_type(), pbHeader.subpacket_sizes_size() ); 
	TRecvPacketPtr rv( packet );
	
	for(int packetNum = 0; packetNum < pbHeader.subpacket_sizes_size( ); packetNum++ ) {
		unsigned int subpacketSize = pbHeader.subpacket_sizes( packetNum );
		TWriteStream stream = packet->WriteSubpacket( packetNum, subpacketSize );
		unsigned int done = 0;
		void *currentBlock = NULL;
		int currentBlockSize = 0;
		while( (done < subpacketSize) && stream->Next( &currentBlock, &currentBlockSize ) ) {
			RecvByVal( reinterpret_cast<char*>(currentBlock), currentBlockSize );
			done += currentBlockSize;
		}
		if( done < subpacketSize )
			throw std::runtime_error( "stream->Next failed in syncro::client::Connection::RecvProtocolBuffer");
	}
	return rv;
}

void Connection::DoHandshake() {
	//TODO: Perform a handshake with authentication
	pb::HandshakeRequest oRequest;
	oRequest.set_client_ver_major(0);
	oRequest.set_client_ver_major(6);
	oRequest.set_magic( comms::HANDSHAKE_REQUEST_MAGIC );
	oRequest.set_username( m_serverDetails.m_username );
	oRequest.set_password( m_serverDetails.m_password );
	
	SendProtocolBuffer( comms::packet_types::HandshakeRequest , oRequest );

	TRecvPacketPtr responsePacket = RecvProtocolBuffer(
		comms::packet_types::HandshakeResponse, 1
		);
	pb::HandshakeResponse response;
	response.ParsePartialFromZeroCopyStream( responsePacket->ReadSubpacket( 0 ).get() );
	if( !response.has_magic() || response.magic().compare( comms::HANDSHAKE_RESPONSE_MAGIC ) != 0 ) {
		throw std::runtime_error( "Handshake response magic incorrect");
	}
	if( response.has_uuid() ) {
		m_uuid = response.uuid();
	}
	//TODO: Check for authentication success/failure, and save the uuid?
}

void Connection::GetFolderList(FolderList& list) {
	pb::FolderListRequest request;
	request.set_search_string("blah");

	SendProtocolBuffer( comms::packet_types::FolderListRequest, request );

	TRecvPacketPtr responsePacket = RecvProtocolBuffer(
		comms::packet_types::FolderListResponse, 1
		);
	pb::FolderList response;
	response.ParseFromZeroCopyStream( responsePacket->ReadSubpacket( 0 ).get() );
	list.resize( response.folders_size() );
	for( int folderNum = 0; folderNum < response.folders_size(); folderNum++ ) {
		const pb::FolderInfo& folder = response.folders( folderNum );
		if( folder.has_folder_id() )
			list[ folderNum ].Id = folder.folder_id();
		if( folder.has_folder_name() )
			list[ folderNum ].Name = folder.folder_name();
		//TODO: Fill these 2 in at some point...
		if( folder.has_can_read() ) {};
		if( folder.has_can_write() ) {};	
	}
	//return const_cast<const FolderList>(rv);
}

void Connection::SendAdminCommand( const std::string& command, const std::string& param ) {
	pb::GenericAdminCommand request;
	request.set_command( command );
	request.set_param( param );

	SendProtocolBuffer( comms::packet_types::AdminGenericCommand, request );

	TRecvPacketPtr responsePacket = RecvProtocolBuffer( 
		comms::packet_types::AdminAck, 1 
		);
	pb::AdminAck ack;
	ack.ParseFromZeroCopyStream( responsePacket->ReadSubpacket(0).get() );
	if( ack.has_ok() ) {
		if( !ack.ok() ) {
			//TODO: Add better error reporting etc.  maybe by return value rather than exception?
			throw std::runtime_error( "Admin command failed" );
		}
	}
}

void Connection::UploadFile(const UploadFileDetails& details)
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
			responsePacket->ReadSubpacket(0).get() 
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
		std::ifstream file( details.m_localPath.c_str() );
		bool start = true;
		file.seekg( 0, std::ios::end );
		std::streamsize totalSize = file.tellg();
		file.seekg( 0, std::ios::beg );
		bool done = false;
		while( !done && !error && !file.fail() ) {
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
			if( (currPos + sizeToRead) > totalSize )
			{
				header.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_END );
				sizeToRead = static_cast<unsigned int>(totalSize) - currPos;
				done = true;
			}
			TSendPacketList subpackets;
			subpackets.push_back( ProtocolBufferPacket::Create( header ) );
			subpackets.push_back( FileStreamPacket::Create( file, sizeToRead ) );
			SendProtocolBuffer( comms::packet_types::BinaryIncomingData, subpackets );
			TRecvPacketPtr responsePacket = 
				RecvProtocolBuffer( comms::packet_types::BinaryIncomingDataAck, 1 );
			pb::BinaryIncomingAck ack;
			ack.ParseFromZeroCopyStream( responsePacket->ReadSubpacket(0).get() );
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
