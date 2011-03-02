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
	CBasePBResponse::TPointer operator()(
		InputStreamListPtr inputStreams,
		server::UserSession& session
		)
	{
		return CBasePBResponse::TPointer( 
			new CBinaryDataResponse( *( session.GetCurrentSendData() ) ) 
			);
	}
};

class BinaryRequestHandler
{
public:
	CBasePBResponse::TPointer operator()(
		InputStreamListPtr inputStreams,
		server::UserSession& session
		)
	{
		CBinaryDataRequest oRequest( *inputStreams );
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
			return CBasePBResponse::TPointer(
				new SimplePBResponse( 
					comms::packet_types::BinaryRequestRejected 
					)
				);
		}
		session.GetCurrentSendData().reset(
			new CFileSendData(
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

CBinaryDataResponse::CBinaryDataResponse( CFileSendData& inoFileData )
	: m_oFileData( inoFileData )
{
	m_oPacketHeader.set_file_offset( m_oFileData.GetFilePosition() );
	m_oPacketHeader.set_file_size( m_oFileData.GetFileSize() );
	m_oPacketHeader.set_hash_size( 0 );
	if( inoFileData.IsStartFile() )
		m_oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_START );
	else
		m_oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_MIDDLE );

	if( m_oFileData.IsFileFinishedAfterChunk( m_oFileData.GetChunkSize() ) )
		m_oPacketHeader.set_binary_packet_type( pb::BinaryPacketHeader_SectionType_END );
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
		return m_oPacketHeader.ByteSize();
	}
	else if( subpacket == 1 )
	{
		return m_oFileData.GetChunkSize();
	}
	throw std::exception();
}

void CBinaryDataResponse::WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream )
{
	if( inSubpacketIndex == 0 )
		WriteMessage( m_oPacketHeader, stream );
	else
	{
		m_oFileData.FillBuffer( stream );
	}
}

}	// namespace pbHandlers
}	// namespace syncro
