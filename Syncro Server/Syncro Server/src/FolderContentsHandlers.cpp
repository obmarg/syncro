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

#include "FolderContentsHandlers.h"
#include "FolderMan.h"
#include "Folder.h"
#include "SimplePBResponse.h"
#include "UserSession.h"
#include "SyncroPBResponseFactory.h"
#include "libsyncro/packet_types.h"
#include "libsyncro/protocol_buffers/folders.pb.h"
#include <boost/foreach.hpp>

namespace syncro {
namespace pbHandlers {

class FolderContentsHandler {
public:
	CBasePBResponse::TPointer operator()(
		InputStreamListPtr inputStreams,
		server::UserSession& session
		)
	{
		pbHandlers::FolderContentsRequest request(
			*inputStreams,
			session.GetFolderMan()
			);
		return request.GetResponse();
	}
};

static const server::RegisterSessionResponse registerFolderContentsHandler(
	comms::packet_types::FolderContentsRequest,
	FolderContentsHandler()
	);

FolderContentsRequest::FolderContentsRequest( 
	const InputStreamList& inaInputStreams,
	CFolderMan& folderMan
	)
{
	if( inaInputStreams.size() != 1 )
	{
		throw std::runtime_error( 
			"Invalid stream count in FolderContentsRequest constructor"
			);
	}
	pb::FolderContentsRequest request;
	request.ParseFromZeroCopyStream( inaInputStreams[0] );

	m_rootFolder = folderMan.GetFolder( request.folder_id() );
}

CBasePBResponse::TPointer FolderContentsRequest::GetResponse()
{
	SimplePBResponse::MessagePtr msg;
	msg.reset( new pb::FolderContents );

	pb::FolderContents* ptr =
		dynamic_cast< pb::FolderContents* >( msg.get() );

	ProcessFolder( ptr, m_rootFolder );

	CBasePBResponse::TPointer rv;
	rv.reset(
		new SimplePBResponse(
			comms::packet_types::FolderContentsResponse,
			msg
			)
		);

	return rv;
}

void FolderContentsRequest::ProcessFolder( 
	pb::FolderContents* pb, 
	const CFolderPtr folder 
	)
{
	const CFolder::TFileList& files = folder->GetFiles();
	BOOST_FOREACH( const CFolder::sFileData& fileInfo, files )
	{
		pb::FileInfo* pbFileInfo = pb->add_files();
		pbFileInfo->set_name( fileInfo.name );
		pbFileInfo->set_size( fileInfo.size );
	}
	const CFolder::TFolderList& children = folder->GetChildren();
	BOOST_FOREACH( const CFolderPtr& child, children )
	{
		pb::FolderContents* pbChild = pb->add_subfolders();
		pbChild->set_name( child->GetPath() );
		ProcessFolder( pbChild, child );
	}
}

}	// namespace pbHandlers
}	// namespace pb
