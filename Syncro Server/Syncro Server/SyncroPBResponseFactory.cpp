#include "SyncroPBResponseFactory.h"
#include "BinaryDataResponse.h"
#include "BinaryDataRequest.h"

namespace syncro {

CSyncroPBResponseFactory::CSyncroPBResponseFactory() : m_oFolderMan( "C:\\SyncFiles\\" ) {

}

CBasePBResponse::TPointer CSyncroPBResponseFactory::CreateResponse(const unsigned int innPacketType, TInputStreamList& inaInputStreams) {
	
	switch( innPacketType ) {
	case eSyncroPBPacketTypes_BinaryRequest: {
		CBinaryDataRequest oRequest( inaInputStreams );
		std::string sActualFilename = m_oFolderMan.GetFileName( oRequest.GetFolderId(), oRequest.GetFilename() );
		m_pCurrentSendData.reset( new CFileSendData( sActualFilename, oRequest.GetBufferSize() ) );
		}
		//Fall through
	case eSyncroPBPacketTypes_BinaryContinue:
		return CBasePBResponse::TPointer( new CBinaryDataResponse( (*m_pCurrentSendData) ) );
	}
	return CBasePBResponse::TPointer();
}


};	//namespace syncro