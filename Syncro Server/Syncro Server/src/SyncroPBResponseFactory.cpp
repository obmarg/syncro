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
#include "FileHashHandlers.h"
#include <libsyncro/packet_types.h>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/bind.hpp>

namespace syncro
{

// Uncomment to enable PB packet debug output
//#define PB_PACKET_DEBUG

CSyncroPBResponseFactory::CSyncroPBResponseFactory()
{
	//TODO: make this db loading stuff better
	kode::db::Database::TPointer oDB = CSyncroDB::OpenDB( );
	m_pFolderMan.reset( new CFolderMan( oDB ) );
	m_fAuthenticated = false;
	m_pAdminCommandMan.reset( new CAdminCommandManager() );
}

CBasePBResponse::TPointer CSyncroPBResponseFactory::CreateResponse( const unsigned int innPacketType, TInputStreamList& inaInputStreams )
{
	using namespace comms;
	using boost::numeric_cast;

	//TODO: Add in admin mode control messages etc.
	if( !m_fAuthenticated )
	{
		if(
		    ( innPacketType != packet_types::HandshakeRequest ) &&
		    ( innPacketType != packet_types::SaltRequest )
		)
			throw authentication_exception( "Not authenticated" );
	}
#ifdef PB_PACKET_DEBUG
	packet_types::ePBPacketTypes packetType =
	    numeric_cast< packet_types::ePBPacketTypes >( innPacketType );
	std::cout <<
	          "Received " <<
	          packet_types::Str( packetType ) <<
	          " packet\n";
#endif
	switch( innPacketType )
	{
	case packet_types::BinaryRequest:
	{
		CBinaryDataRequest oRequest( inaInputStreams );
		FileTransferDetails details;
		bool fAccept = m_pFolderMan->FileRequested( oRequest, details );
		if( !fAccept )
		{
			break;
		}
		m_pCurrentSendData.reset(
		    new CFileSendData(
		        details.Filename(),
		        oRequest.GetBufferSize(),
		        details.CompletionCallback(),
		        oRequest.GetStartOffset()
		    )
		);
	}
	//Fall through
	case packet_types::BinaryContinue:
		return CBasePBResponse::TPointer( new CBinaryDataResponse(( *m_pCurrentSendData ) ) );
	case packet_types::HandshakeRequest:
	{
		CPBHandshakeRequest oRequest( inaInputStreams );
		CBasePBResponse::TPointer pResponse = oRequest.GetResponse();
		if( oRequest.HasAuthDetails() )
		{
			m_oAuthToken = m_authMan.Authenticate( oRequest.GetUsername(), oRequest.GetPassword(), "" );
		}
		else
		{
			m_oAuthToken = m_authMan.DefaultAuth();
		}
		//if we've got this far and haven't thrown, we're authed
		m_fAuthenticated = true;
		return pResponse;
	}
	case packet_types::BinaryIncomingRequest:
	{
		CBinaryDataRequest oRequest( inaInputStreams );
		FileTransferDetails details;
		bool fAccept = m_pFolderMan->IncomingFile( oRequest, details );
		if( fAccept )
		{
			m_pCurrentRecvData.reset(
			    new CBinaryIncomingData(
			        details.Filename(),
			        details.CompletionCallback()
			    )
			);
			std::cout << "Accepted Binary Incoming Request\n";
			std::cout << "Filename: " << details.Filename().c_str() << "\n";
			std::cout << "Current Size: " << details.CurrentSize() << "\n";
		}
		return CBinaryIncomingResponse::Create( 
			CBinaryIncomingResponse::eResponseType_Response, 
			fAccept,
			details.CurrentSize()
			);
	}
	case packet_types::BinaryIncomingData:
	{
		bool fOK = true;
		try
		{
			m_pCurrentRecvData->HandlePacket( inaInputStreams );
		}
		catch( const std::exception& )
		{
			fOK = false;
		}
		return CBinaryIncomingResponse::Create( CBinaryIncomingResponse::eResponseType_Ack, fOK );
	}
	case packet_types::AdminGenericCommand:
	{
		CAdminCommandHandler oHandler( inaInputStreams, m_oAuthToken, ( *m_pAdminCommandMan ) );
		return oHandler.GetResponse();
	}
	case packet_types::FolderListRequest:
	{
		FolderListRequestHandler request( inaInputStreams );
		return request.GetResponse();
	}
	case packet_types::SaltRequest:
	{
		return CSaltResponse::Create( m_authMan.Salt() );
	}
	case packet_types::FileHashRequest:
	{
		pbHandlers::FileHashRequest request( 
			inaInputStreams, 
			( *m_pFolderMan ),
			boost::bind( 
				&CSyncroPBResponseFactory::HashOkCallback,
				this,
				_1, _2
				)
			);
		return request.GetResponse();
	}
	};

	throw std::runtime_error( "Invalid pb request passed to response factory" );
}

void CSyncroPBResponseFactory::HashOkCallback( 
	const std::string& filename, 
	int64_t sizeHashed 
	)
{
	if( m_pCurrentRecvData )
	{
		if( m_pCurrentRecvData->Filename() == filename )
		{
			//TODO: This might except, should maybe catch it?
			m_pCurrentRecvData->ShouldResume( sizeHashed );
		}
	}
}


};	//namespace syncro
