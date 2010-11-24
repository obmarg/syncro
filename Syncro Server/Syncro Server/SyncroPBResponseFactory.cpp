#include "SyncroPBResponseFactory.h"
#include "BinaryDataResponse.h"
#include "BinaryDataRequest.h"
#include "BinaryIncomingResponse.h"
#include "BinaryIncomingData.h"
#include "HandshakeHandlers.h"
#include "SyncroDB.h"
#include "AuthManager.h"
#include "AdminCommandManager.h"
#include "AdminCommandHandler.h"
#include "FolderListHandlers.h"

namespace syncro {

CSyncroPBResponseFactory::CSyncroPBResponseFactory() {
	//TODO: make this db loading stuff better
	kode::db::Database::TPointer oDB = CSyncroDB::OpenDB( );
	m_pFolderMan.reset( new CFolderMan( oDB ) );
	m_fAuthenticated = false;
	m_pAdminCommandMan.reset( new CAdminCommandManager() );
}

CBasePBResponse::TPointer CSyncroPBResponseFactory::CreateResponse(const unsigned int innPacketType, TInputStreamList& inaInputStreams) {
	using namespace comms;

	//TODO: Add in admin mode control messages etc.
	if( (!m_fAuthenticated) && (innPacketType != packet_types::HandshakeRequest) ) {
		throw authentication_exception("Not authenticated");
	}
	switch( innPacketType ) {
	case packet_types::BinaryRequest: {
			CBinaryDataRequest oRequest( inaInputStreams );
			std::string sActualFilename = m_pFolderMan->GetFileName( oRequest.GetFolderId(), oRequest.GetFilename() );
			m_pCurrentSendData.reset( new CFileSendData( sActualFilename, oRequest.GetBufferSize() ) );
		}
		//Fall through
	case packet_types::BinaryContinue:
		return CBasePBResponse::TPointer( new CBinaryDataResponse( (*m_pCurrentSendData) ) );
	case packet_types::HandshakeRequest: {
			CPBHandshakeRequest oRequest( inaInputStreams );
			CBasePBResponse::TPointer pResponse = oRequest.GetResponse();
			CAuthManager oAuthMan;
			if( !oRequest.HasAuthDetails() && oAuthMan.NeedsAuth() )
				throw authentication_exception("Authentication required but no details provided");
			else if( oRequest.HasAuthDetails() ) {
				m_oAuthToken = oAuthMan.Authenticate( oRequest.GetUsername(), oRequest.GetPassword(), "" );
			}
			//if we've got this far and haven't thrown, we're authed
			m_fAuthenticated = true;
			return pResponse;
		}
	case packet_types::BinaryIncomingRequest: {
			CBinaryDataRequest oRequest( inaInputStreams );
			std::string sFilename = m_pFolderMan->IncomingFile( oRequest );
			bool fAccept = !sFilename.empty();
			m_pCurrentRecvData.reset( new CBinaryIncomingData( sFilename ) );
			return CBinaryIncomingResponse::Create( CBinaryIncomingResponse::eResponseType_Response, fAccept );
		}
	case packet_types::BinaryIncomingData: {
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
	case packet_types::AdminGenericCommand: {
			CAdminCommandHandler oHandler( inaInputStreams, m_oAuthToken, (*m_pAdminCommandMan) );
			return oHandler.GetResponse();
		}
	case packet_types::FolderListRequest: {
			FolderListRequestHandler request( inaInputStreams );
			return request.GetResponse();
		}
	};
	
	throw std::runtime_error("Invalid pb request passed to response factory");
}


};	//namespace syncro
