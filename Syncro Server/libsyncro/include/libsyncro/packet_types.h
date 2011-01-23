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

#ifndef _SYNCRO_PACKET_TYPES_H_
#define _SYNCRO_PACKET_TYPES_H_

#include <string>

namespace syncro
{
namespace comms
{
namespace packet_types
{

enum ePBPacketTypes
{
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
	SaltRequest,				// 14
	SaltResponse,				// 15
	FileHashRequest,			// 16
	FileHashResponse			// 17
};

extern std::string Str( ePBPacketTypes );

}	// namespace packet_types
}	// namespace comms
}	// namespace syncro

#endif