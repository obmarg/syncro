#include "packet_types.h"
#include <boost/lexical_cast.hpp>

namespace syncro {
namespace comms {
namespace packet_types {

std::string Str( ePBPacketTypes type ) {
	switch( type )
	{
	case BinaryRequest:
		return "Binary Request";
	case BinaryContinue:
		return "Binary Continue";
	case BinaryResponse:
		return "Binary Response";
	case HandshakeRequest:
		return "Handshake Request";
	case HandshakeResponse:
		return "Handshake Response";
	case BinaryIncomingRequest:
		return "Binary Incoming Request";
	case BinaryIncomingResponse:
		return "Binary Incoming Response";
	case BinaryIncomingData:
		return "Binary Incoming Data";
	case BinaryIncomingDataAck:
		return "Binary Incoming Data Ack";
	case AdminGenericCommand:
		return "Admin Generic Command";
	case AdminAck:
		return "Admin Ack";
	case FolderListRequest:
		return "Folder List Request";
	case FolderListResponse:
		return "Folder List Response";
	case SaltRequest:
		return "Salt Request";
	case SaltResponse:
		return "Salt Response";
	default:
		return "Unknown (" + boost::lexical_cast<std::string>( type ) + ")";
	}
}

}	// namespace packet_types
}	// namespace comms
}	// namespace kode