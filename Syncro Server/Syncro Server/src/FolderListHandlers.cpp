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

#include "FolderListHandlers.h"
#include "SyncroDB.h"
#include "FolderMan.h"
#include "UserSession.h"
#include "ResponseFunctions.h"
#include "SyncroPBResponseFactory.h"
#include <libsyncro/folderlist.h>
#include <libsyncro/packet_types.h>
#include <libsyncro/protocol_buffers/folders.pb.h>
#include <boost/foreach.hpp>

namespace syncro {
namespace pbHandlers {

static BasePBResponse::TPointer GetFolderListResponse(
	InputStreamListPtr inputStreams,
	server::UserSession& session
	)
{
	pb::FolderListRequest request;
	request.ParseFromZeroCopyStream( (*inputStreams)[0] );
	//TODO: At some point start doing stuff with the request.  
	//		for now we just ignore everything

	const FolderList& folders = session.GetFolderMan().GetFoldersInfo();
	BasePBResponse::TPointer rv( new FolderListResponse( folders ) );
	return rv;
}

static const server::RegisterSessionResponse folderListRegister(
	comms::packet_types::FolderListRequest,
	&GetFolderListResponse
	);

FolderListResponse::FolderListResponse( const FolderList& list )
{
	if( list.empty() )
	{
		m_response.set_empty( true );
	}
	else
	{
		BOOST_FOREACH( const FolderInfo & folder, list )
		{
			pb::FolderInfo* info = m_response.add_folders();
			info->set_folder_id( folder.Id );
			info->set_folder_name( folder.Name );
			info->set_folder_path( folder.Path );
		}
	}
}

}	// namespace pbHandlers
}	// namespace syncro
