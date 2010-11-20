#include "FolderListHandlers.h"
#include "SyncroDB.h"
#include "FolderMan.h"
#include <libsyncro/folderlist.h>
#include <libsyncro/packet_types.h>
#include <libsyncro/protocol_buffers/folders.pb.h>

namespace syncro {

class FolderListResponse : public CBasePBResponse {
public:
	FolderListResponse(const FolderList& list) {
		foreach( const FolderInfo& folder, list ) {
			pb::FolderInfo* info = m_response.add_folders();
			info->set_folder_id( folder.Id );
			info->set_folder_name( folder.Name );
		}
	}

	virtual std::vector<unsigned int> GetSubpacketSizes() {
		std::vector<unsigned int> rv( 1 );
		rv[0] = m_response.ByteSize();
		return rv;
	}
	virtual unsigned int GetSubpacketCount() { return 1; };

	virtual unsigned int GetPacketType() {
		return comms::packet_types::FolderListResponse;
	}

	virtual void WriteSubpacket(int inSubpacketIndex,google::protobuf::io::ZeroCopyOutputStream& stream) {
		if( inSubpacketIndex != 0 )
			throw std::logic_error( "Attempted to write imaginary subpacket in FolderListResponse::WriteSubpacket" );
		WriteMessage( m_response, stream );
	}
private:
	pb::FolderList m_response;
};

FolderListRequestHandler::FolderListRequestHandler(TInputStreamList& inaInputStreams) {
	pb::FolderListRequest request;
	request.ParseFromZeroCopyStream( inaInputStreams[0] );
	//TODO: At some point start doing stuff with the request.  for now we just ignore everything
}

CBasePBResponse::TPointer FolderListRequestHandler::GetResponse() {
	kode::db::Database::TPointer db = CSyncroDB::OpenDB( );
	CFolderMan folderMan( db );
	const FolderList& folders = folderMan.GetFoldersInfo();
	CBasePBResponse::TPointer rv( new FolderListResponse(folders) );
	return rv;
}

}	// namespace syncro
