#include "connection.h"
#include "comms.h"
#include "protocol_buffers/handshake.pb.h"
#include "protocol_buffers/header.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <stdexcept>

namespace syncro {
namespace client {

const size_t RECV_BUFFER_SIZE=512;

Connection::Connection( const ConnectionDetails& details ) :
	TCPConnection( details.m_host, details.m_port ),
	m_serverDetails( details )
{
	DoHandshake();
}

void 
Connection::SendProtocolBuffer( const google::protobuf::MessageLite& message ) {
	std::vector<unsigned char> aBuffer( message.ByteSize() + comms::PacketHeader::BYTE_SIZE );

	//TODO: Need to add pb::PacketHeader support to this function (or remove it from the other parts of the protocol)

	comms::PacketHeader()
		.SetFirstByte( comms::PB_REQUEST_FIRST_BYTE )
		.SetPacketSize( message.ByteSize() )
		.Write( aBuffer );
	
	google::protobuf::io::ArrayOutputStream oStream( 
		&aBuffer[ comms::PacketHeader::BYTE_SIZE ],
		message.ByteSize(),
		message.ByteSize()
		);

	message.SerializeToZeroCopyStream( &oStream );

	Send( aBuffer );
}

void 
Connection::RecvProtocolBuffer( google::protobuf::MessageLite& message ) {
	std::vector<unsigned char> aBuffer( RECV_BUFFER_SIZE );
	
	Recv( aBuffer, comms::PacketHeader::BYTE_SIZE );
	comms::PacketHeader oFirstHeader;
	oFirstHeader.Read( &aBuffer[0] );

	if( oFirstHeader.FirstByte() != comms::PB_RESPONSE_FIRST_BYTE ) {
		throw std::runtime_error( "Invalid first byte in syncro::client::connection::RecvProtocolBuffer");
	}
	unsigned int pbPacketHeaderSize = oFirstHeader.PacketSize();
	
	std::vector<unsigned char> pbHeaderBuffer( pbPacketHeaderSize );
	Recv( pbHeaderBuffer, pbPacketHeaderSize );
	message.ParseFromArray( &pbHeaderBuffer[0], pbPacketHeaderSize );
}

void
Connection::RecvProtocolBuffer( google::protobuf::MessageLite& message, unsigned int recvSize ) {
	std::vector<unsigned char> aBuffer( recvSize );
	Recv( aBuffer, recvSize );
	message.ParseFromArray( &aBuffer[0], recvSize );
}


void Connection::DoHandshake() {
	//TODO: Perform a handshake with authentication
	pb::HandshakeRequest oRequest;
	oRequest.set_client_ver_major(0);
	oRequest.set_client_ver_major(6);
	oRequest.set_magic( comms::HANDSHAKE_REQUEST_MAGIC );
	oRequest.set_username( m_serverDetails.m_username );
	oRequest.set_password( m_serverDetails.m_password );
	
	SendProtocolBuffer( oRequest );

	pb::PacketHeader oResponseHeader;
	RecvProtocolBuffer( oResponseHeader );
	//TODO: Finish this method to get the handshake response
}


}
}