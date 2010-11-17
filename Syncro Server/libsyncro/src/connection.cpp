#include "connection.h"
#include "comms.h"
#include "protocol_buffers/handshake.pb.h"
#include "protocol_buffers/header.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <stdexcept>
#include <boost/shared_array.hpp>
#include <boost/scoped_array.hpp>
#include "packet_types.h"


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
	  m_buffers( numSubpackets ),
	  m_packetType( packetType )
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

const size_t RECV_BUFFER_SIZE=512;

Connection::Connection( const ConnectionDetails& details ) :
	TCPConnection( details.m_host, details.m_port ),
	m_serverDetails( details )
{
	DoHandshake();
}

void 
Connection::SendProtocolBuffer( uint32_t packetType, const google::protobuf::MessageLite& message ) {
	pb::PacketHeader pbHeader;
	pbHeader.set_packet_type(packetType);
	pbHeader.set_subpacket_sizes( 0, message.ByteSize() );
	
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
			RecvByVal( reinterpret_cast<char*>(currentBlock), currentBlockSize, currentBlockSize );
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

	TRecvPacketPtr responsePacket = RecvProtocolBuffer();
	if( responsePacket->GetPacketType() != comms::packet_types::HandshakeResponse ) {
		throw std::runtime_error( "Handshake request not replied to");
	}
	if( responsePacket->GetNumSubpackets() != 1 ) {
		throw std::runtime_error( "Incorrect number of handshake response subpackets");
	}
	pb::HandshakeResponse response;
	response.ParsePartialFromZeroCopyStream( responsePacket->ReadSubpacket( 0 ).get() );
	if( !response.has_magic() || response.magic().compare( comms::HANDSHAKE_RESPONSE_MAGIC ) != 0 ) {
		throw std::runtime_error( "Handshake response magic incorrect");
	}
	//TODO: Check for authentication success/failure, and save the uuid?
}


}
}