#ifndef _BINARY_DATA_REQUEST_H_
#define _BINARY_DATA_REQUEST_H_

#include "common.h"
#include "protocol_buffers\binarydata.pb.h"


namespace syncro {

class CBinaryDataRequest {
public:
	CBinaryDataRequest(TInputStreamList& inaInputStreams) {
		pb::BinaryDataRequest oRequest;
		if( inaInputStreams.size() == 1 ) {
			if( !oRequest.ParseFromZeroCopyStream( inaInputStreams[0] ) )
				throw std::exception( "CBinaryDataRequestHandler: invalid BinaryRequestData packet passed in" );
			m_sFilename = oRequest.file_name();
			m_nFolderId = oRequest.folder_id();
		} 
	}
	const std::string& GetFilename() const { return m_sFilename; };
	const int GetFolderId() const { return m_nFolderId; };
private:
	std::string m_sFilename;
	int m_nFolderId;
};

};	//namespace syncro

#endif