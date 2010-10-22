#include "SyncroPBResponseFactory.h"
#include "BinaryDataResponse.h"
#include "BinaryDataRequest.h"
#include "HandshakeHandlers.h"
#include "SyncroDB.h"

namespace syncro {

CSyncroPBResponseFactory::CSyncroPBResponseFactory() {
	//TODO: make this db loading stuff better
	Database::TPointer oDB = CSyncroDB::OpenDB( );
	m_pFolderMan.reset( new CFolderMan( oDB ) );
	m_fAuthenticated = false;
}

CBasePBResponse::TPointer CSyncroPBResponseFactory::CreateResponse(const unsigned int innPacketType, TInputStreamList& inaInputStreams) {
	if( (!m_fAuthenticated) && (innPacketType != eSyncroPBPacketTypes_HandshakeRequest) ) {
		throw authentication_exception("Not authenticated");
	}
	switch( innPacketType ) {
	case eSyncroPBPacketTypes_BinaryRequest: {
			CBinaryDataRequest oRequest( inaInputStreams );
			std::string sActualFilename = m_pFolderMan->GetFileName( oRequest.GetFolderId(), oRequest.GetFilename() );
			m_pCurrentSendData.reset( new CFileSendData( sActualFilename, oRequest.GetBufferSize() ) );
		}
		//Fall through
	case eSyncroPBPacketTypes_BinaryContinue:
		return CBasePBResponse::TPointer( new CBinaryDataResponse( (*m_pCurrentSendData) ) );
	case eSyncroPBPacketTypes_HandshakeRequest: {
			CPBHandshakeRequest oRequest( inaInputStreams );
			CBasePBResponse::TPointer pResponse = oRequest.GetResponse();
			//if we've got this far and haven't thrown, we're authed
			m_fAuthenticated = true;
			return pResponse;
		}
	}
	throw std::runtime_error("Invalid pb request passed to response factory");
}


};	//namespace syncro
