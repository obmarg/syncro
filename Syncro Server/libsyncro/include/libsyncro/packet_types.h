#ifndef _SYNCRO_PACKET_TYPES_H_
#define _SYNCRO_PACKET_TYPES_H_

namespace syncro {
namespace comms {
namespace packet_types {

enum ePBPacketTypes {
	BinaryRequest = 1,			// 1
	BinaryContinue,				// 2
	BinaryResponse,				// 3
	HandshakeRequest,			// 4
	HandshakeResponse,			// 5
	BinaryIncomingRequest,		// 6
	BinaryIncomingResponse,		// 7
	BinaryIncomingData,			// 8
	BinaryIncomingDataAck,		// 9
	AdminGenericCommand,		// 10
	AdminAck,					// 11
	FolderListRequest,			// 12
	FolderListResponse,			// 13
};

}	// namespace packet_types
}	// namespace comms
}	// namespace syncro

#endif