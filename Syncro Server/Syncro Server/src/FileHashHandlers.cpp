#include "FileHashHandlers.h"
#include "SimplePBResponse.h"
#include "BinaryDataRequest.h"
#include "FolderMan.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>
#include <libsyncro/packet_types.h>
#include <kode/base64.h>
#include <cryptopp/sha.h>
#include <boost/numeric/conversion/cast.hpp>
#include <fstream>

namespace syncro {
namespace pbHandlers {

FileHashRequest::FileHashRequest(
	TInputStreamList& inaInputStreams,
	CFolderMan&	folderMan
	) :
m_ok( false )
{
	pb::FileHashRequest hashRequest;
	if( inaInputStreams.size() != 1 )
	{
		throw std::runtime_error( 
			"Invalid number of subpackets passed to "
			"FileHashRequestHandler::FileHashRequestHandler"
			);
	}
	hashRequest.ParseFromZeroCopyStream( inaInputStreams[0] );

	if( !hashRequest.has_data_size() || hashRequest.data_size() == 0)
	{
		return;
	}
	
	FileTransferDetails fileDetails;
	CBinaryDataRequest fileRequest( 
		hashRequest.folder_id(),
		hashRequest.file_name()
		);
	bool foundFile = folderMan.FileRequested( fileRequest, fileDetails );
	if( !foundFile )
	{
		return;
	}
	CryptoPP::SHA hashObject;
	std::ifstream file( 
		fileDetails.Filename().c_str(), 
		std::ios::binary | std::ios::in 
		);
	if( file.fail() )
	{
		return;
	}
	file.seekg( 0, std::ios::end );
	int64_t fileSize = file.tellg();
	file.seekg( 0, std::ios::beg );

	if( hashRequest.has_data_size() )
	{
		if( fileSize < hashRequest.data_size() )
		{
			return;
		}
		fileSize = hashRequest.data_size();
	}

	std::vector<unsigned char> buffer( 1024 );
	do 
	{
		int64_t sizeLeft = fileSize - file.tellg();
		if( sizeLeft == 0 )
			break;

		if( sizeLeft < buffer.size() )
			buffer.resize( 
				boost::numeric_cast< size_t >( sizeLeft )
				);
		file.read( 
			reinterpret_cast< char* >( &buffer[0] ),
			buffer.size()
			);

		hashObject.Update( &buffer[0], buffer.size() );
	} while ( !file.eof() );
	
	unsigned char hashBuffer[ CryptoPP::SHA::DIGESTSIZE ];
	hashObject.Final( hashBuffer );

	std::string hash = kode::base64::Encode( 
		hashBuffer,
		sizeof( hashBuffer )
		);

	m_ok = ( hash == hashRequest.hash() );
}

CBasePBResponse::TPointer 
	FileHashRequest::GetResponse()
{
	SimplePBResponse::MessagePtr msg;
	msg.reset( new pb::FileHashResponse() );
	
	pb::FileHashResponse* ptr = 
		static_cast< pb::FileHashResponse* >( msg.get() );
	ptr->set_ok( m_ok );

	CBasePBResponse::TPointer rv;
	rv.reset( 
		new SimplePBResponse( 
			syncro::comms::packet_types::FileHashResponse, 
			msg 
			)
		);

	return rv;
}

}	// namespace pbHandlers
}	// namespace syncro
