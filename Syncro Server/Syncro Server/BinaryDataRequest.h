#ifndef _BINARY_DATA_REQUEST_H_
#define _BINARY_DATA_REQUEST_H_

#include "common.h"
#include "protocol_buffers/binarydata.pb.h"


namespace syncro {

class CBinaryDataRequest {
public:
	CBinaryDataRequest(TInputStreamList& inaInputStreams) {
		pb::BinaryDataRequest oRequest;
		if( inaInputStreams.size() == 1 ) {
			if( !oRequest.ParseFromZeroCopyStream( inaInputStreams[0] ) )
				throw std::runtime_error( "CBinaryDataRequestHandler: invalid BinaryRequestData packet passed in" );
			m_sFilename = oRequest.file_name();
			m_nFolderId = oRequest.folder_id();
			m_nBufferSize = 0;
			if( oRequest.has_recv_buffer_size() )
				m_nBufferSize = oRequest.recv_buffer_size();
			if( oRequest.has_file_size() )
				m_nFileSize = oRequest.file_size();
			else
				m_nFileSize = -1;
		} 
	}
	~CBinaryDataRequest() {};
	const std::string& GetFilename() const { return m_sFilename; };
	const int GetFolderId() const { return m_nFolderId; };
	const int GetBufferSize() const { return m_nBufferSize; };
	const int GetFileSize() const { return m_nFileSize; };
private:
	std::string m_sFilename;
	int m_nFolderId;
	int m_nBufferSize;
	int m_nFileSize;
};

};	//namespace syncro

#endif
