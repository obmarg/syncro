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

#include "packet_types.h"
#include <boost/lexical_cast.hpp>

namespace syncro
{
namespace comms
{
namespace packet_types
{

std::string Str( ePBPacketTypes type )
{
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
	case FileHashRequest:
		return "File Hash Request";
	case FileHashResponse:
		return "File Hash Response";
	default:
		return "Unknown (" + boost::lexical_cast<std::string>( type ) + ")";
	}
}

}	// namespace packet_types
}	// namespace comms
}	// namespace kode