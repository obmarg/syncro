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

#include "BinaryIncomingResponse.h"
#include "BinaryIncomingData.h"
#include "BinaryDataRequest.h"
#include "UserSession.h"
#include "ResponseFunctions.h"
#include "Logging.h"
#include <libsyncro/packet_types.h>
#include <iostream>

namespace syncro {
namespace pbHandlers {

class BinaryIncomingRequestHandler {
public:
	BasePBResponse::TPointer operator()(
		InputStreamListPtr inputStreams,
		server::UserSession& session
		)
	{
		BinaryDataRequest request( *inputStreams );
		FileTransferDetails details;
		bool fAccept = 
			session.GetFolderMan().IncomingFile( request, details );
		if( fAccept )
		{
			session.GetCurrentRecvData().reset(
				new BinaryIncomingData(
					details.Filename(),
					details.CompletionCallback()
					)
				);
            log::info << "Accepted Binary Incoming Request\n";
            log::info << "Filename: " << details.Filename().c_str() << "\n";
            log::info << "Current Size: " << details.CurrentSize() << "\n";
		}
		return BinaryIncomingResponse::Create( 
			BinaryIncomingResponse::eResponseType_Response, 
			fAccept,
			details.CurrentSize()
			);
	}
};

static const server::RegisterSessionResponse registerBinaryIncomingRequestHandler(
	comms::packet_types::BinaryIncomingRequest,
	BinaryIncomingRequestHandler()
	);

class BinaryIncomingDataHandler {
public:
	BasePBResponse::TPointer operator()(
		InputStreamListPtr inputStreams,
		server::UserSession& session
		)
	{
		bool fOK = true;
		try
		{
			session.GetCurrentRecvData()->HandlePacket( *inputStreams );
		}
		catch( const std::exception& ex )
		{
			fOK = false;
            log::error << "Caught exception in BinaryIncomingDataHandler: "
				<< ex.what() << "\n";
		}
		return BinaryIncomingResponse::Create( 
			BinaryIncomingResponse::eResponseType_Ack, 
			fOK 
			);
	}
};

static const server::RegisterSessionResponse registerBinaryIncomingDataHandler(
	comms::packet_types::BinaryIncomingData,
	BinaryIncomingDataHandler()
	);

}	// namespace syncro
}	// namespace pbHandlers
