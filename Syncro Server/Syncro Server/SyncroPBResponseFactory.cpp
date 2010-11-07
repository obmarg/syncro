#include "SyncroPBResponseFactory.h"
#include "BinaryDataResponse.h"
#include "BinaryDataRequest.h"
#include "BinaryIncomingResponse.h"
#include "BinaryIncomingData.h"
#include "HandshakeHandlers.h"
#include "SyncroDB.h"
#include "AuthManager.h"

namespace syncro {

CSyncroPBResponseFactory::CSyncroPBResponseFactory() {
	//TODO: make this db loading stuff better
	db::Database::TPointer oDB = CSyncroDB::OpenDB( );
	m_pFolderMan.reset( new CFolderMan( oDB ) );
	m_fAuthenticated = false;
}

CBasePBResponse::TPointer CSyncroPBResponseFactory::CreateResponse(const unsigned int innPacketType, TInputStreamList& inaInputStreams) {
	//TODO: Add in admin mode control messages etc.
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
			CAuthManager oAuthMan;
			if( !oRequest.HasAuthDetails() && oAuthMan.NeedsAuth() )
				throw authentication_exception("Authentication required but no details provided");
			else if( oRequest.HasAuthDetails() ) {
				oAuthMan.Authenticate( oRequest.GetUsername(), oRequest.GetPassword() );
			}
			//if we've got this far and haven't thrown, we're authed
			m_fAuthenticated = true;
			return pResponse;
		}
	case eSyncroPBPacketTypes_BinaryIncomingRequest: {
			CBinaryDataRequest oRequest( inaInputStreams );
			std::string sFilename = m_pFolderMan->IncomingFile( oRequest );
			bool fAccept = !sFilename.empty();
			m_pCurrentRecvData.reset( new CBinaryIncomingData( sFilename ) );
			return CBinaryIncomingResponse::Create( CBinaryIncomingResponse::eResponseType_Response, fAccept );
		}
	case eSyncroPBPacketTypes_BinaryIncomingData: {
			bool fOK = true;
			try {
				m_pCurrentRecvData->HandlePacket( inaInputStreams );
			}
			catch( const std::exception& ex ) 
			{
				fOK = false;
			}
			return CBinaryIncomingResponse::Create( CBinaryIncomingResponse::eResponseType_Ack, fOK );
		}
	};
	throw std::runtime_error("Invalid pb request passed to response factory");
}


};	//namespace syncro
