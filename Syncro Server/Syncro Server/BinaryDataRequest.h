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
		} 
	}
	const std::string& GetFilename() const { return m_sFilename; };
private:
	std::string m_sFilename;
};

};	//namespace syncro

#endif