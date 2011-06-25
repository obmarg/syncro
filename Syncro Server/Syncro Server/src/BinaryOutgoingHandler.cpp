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

#include "BinaryDataRequest.h"
#include "BinaryOutgoingHandler.h"
#include "SimplePBResponse.h"
#include <libsyncro/packet_types.h>

namespace syncro {
namespace pbHandlers {

class BinaryDataHandler
{
public:
	BasePBResponse::TPointer operator()(
		InputStreamListPtr inputStreams,
		server::UserSession& session
		)
	{
		return BasePBResponse::TPointer( 
			new CBinaryDataResponse( *( session.GetCurrentSendData() ) ) 
			);
	}
};

class BinaryRequestHandler
{
public:
	BasePBResponse::TPointer operator()(
		InputStreamListPtr inputStreams,
		server::UserSession& session
		)
	{
		BinaryDataRequest oRequest( *inputStreams );
		FileTransferDetails details;
		bool fAccept = 
			session.GetFolderMan().FileRequested( oRequest, details );
		if( !fAccept )
		{
#ifdef _DEBUG
			//TODO: Maybe remove this once finished debugging
			std::cout << "Binary Request Rejected.\n" <<
				"FolderId: " <<		oRequest.GetFolderId()		<< "\n" <<
				"Filename: " <<	oRequest.GetFilename().c_str()	<< "\n" <<
				"Filesize: " <<		oRequest.GetFileSize()		<< "\n" <<
				"Start Offset:" <<	oRequest.GetStartOffset()	<< "\n";
#endif
			return BasePBResponse::TPointer(
				new SimplePBResponse( 
					comms::packet_types::BinaryRequestRejected 
					)
				);
		}
		session.GetCurrentSendData().reset(
			new FileSendData(
				details.Filename(),
				oRequest.GetBufferSize(),
				details.CompletionCallback(),
				oRequest.GetStartOffset()
				)	
			);
		return BinaryDataHandler()( 0, session );
	}
};

static const server::RegisterSessionResponse binaryRequestRegister(
	comms::packet_types::BinaryRequest,
	BinaryRequestHandler()
	);

static const server::RegisterSessionResponse binaryDataRegister(
	comms::packet_types::BinaryContinue,
	BinaryDataHandler()
	);

CBinaryDataResponse::CBinaryDataResponse( FileSendData& inoFileData ) :
m_fileData( inoFileData )
{
	m_packetHeader.set_file_offset( m_fileData.GetFilePosition() );
	m_packetHeader.set_file_size( m_fileData.GetFileSize() );
	m_packetHeader.set_hash_size( 0 );
	if( inoFileData.IsStartFile() )
	{
		m_packetHeader.set_binary_packet_type( 
			pb::BinaryPacketHeader_SectionType_START 
			);
	}
	else
	{
		m_packetHeader.set_binary_packet_type( 
			pb::BinaryPacketHeader_SectionType_MIDDLE 
			);
	}

	if( m_fileData.IsFileFinishedAfterChunk( m_fileData.GetChunkSize() ) )
	{
		m_packetHeader.set_binary_packet_type( 
			pb::BinaryPacketHeader_SectionType_END 
			);
		m_packetHeader.set_modified_time( 
			m_fileData.GetFileModifiedTime() 
			);
	}
}

CBinaryDataResponse::~CBinaryDataResponse()
{

}

unsigned int CBinaryDataResponse::GetSubpacketCount()
{
	return 2;
}

uint32_t CBinaryDataResponse::GetSubpacketSize( uint32_t subpacket )
{
	assert( subpacket == 0 || subpacket == 1 );
	if( subpacket == 0 )
	{
		return m_packetHeader.ByteSize();
	}
	else if( subpacket == 1 )
	{
		return m_fileData.GetChunkSize();
	}
	throw std::exception();
}

void CBinaryDataResponse::WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream )
{
	if( inSubpacketIndex == 0 )
		WriteMessage( m_packetHeader, stream );
	else
	{
		m_fileData.FillBuffer( stream );
	}
}

}	// namespace pbHandlers
}	// namespace syncro
