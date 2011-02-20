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

#include "FileHashHandlers.h"
#include "SimplePBResponse.h"
#include "BinaryDataRequest.h"
#include "FolderMan.h"
#include <libsyncro/protocol_buffers/binarydata.pb.h>
#include <libsyncro/packet_types.h>
#include <kode/base64.h>
#include <kode/hashutils.h>
#include <cryptopp/sha.h>
#include <boost/numeric/conversion/cast.hpp>
#include <fstream>

namespace syncro
{
namespace pbHandlers
{

FileHashRequest::FileHashRequest(
    TInputStreamList& inaInputStreams,
    CFolderMan&	folderMan,
	HashOkCallback callback
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

	if( !hashRequest.has_data_size() || hashRequest.data_size() == 0 )
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

	kode::HashPtr hashObj = 
		kode::HashFile< CryptoPP::SHA >( fileDetails.Filename().c_str() );

	std::string hash = kode::base64::Encode(
	                       hashObj->GetHash(),
	                       hashObj->Size()
	                   );

	m_ok = ( hash == hashRequest.hash() );

	if( m_ok && callback )
	{
		callback( fileDetails.Filename(), hashRequest.data_size() );
	}
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
