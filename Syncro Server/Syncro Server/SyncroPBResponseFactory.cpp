#include "SyncroPBResponseFactory.h"
#include "BinaryDataResponse.h"
#include "BinaryDataRequest.h"
#include "SyncroDB.h"

namespace syncro {

CSyncroPBResponseFactory::CSyncroPBResponseFactory() {
	//TODO: make this db loading stuff better
	Database::TPointer oDB = CSyncroDB::OpenDB( );
	m_pFolderMan.reset( new CFolderMan( oDB ) );
}

CBasePBResponse::TPointer CSyncroPBResponseFactory::CreateResponse(const unsigned int innPacketType, TInputStreamList& inaInputStreams) {
	
	switch( innPacketType ) {
	case eSyncroPBPacketTypes_BinaryRequest: {
		CBinaryDataRequest oRequest( inaInputStreams );
		std::string sActualFilename = m_pFolderMan->GetFileName( oRequest.GetFolderId(), oRequest.GetFilename() );
		m_pCurrentSendData.reset( new CFileSendData( sActualFilename, oRequest.GetBufferSize() ) );
		}
		//Fall through
	case eSyncroPBPacketTypes_BinaryContinue:
		return CBasePBResponse::TPointer( new CBinaryDataResponse( (*m_pCurrentSendData) ) );
	}
	return CBasePBResponse::TPointer();
}


};	//namespace syncro