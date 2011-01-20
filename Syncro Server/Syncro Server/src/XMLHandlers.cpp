#include "XMLHandlers.h"
#include "XMLBuilder.h"
#include "PBRequestHandler.h"
#include "SyncroPBResponseFactory.h"
#include "ServerComms.h"
#include <string>
#include <boost/lexical_cast.hpp>
#include <stdint.h>

namespace syncro
{

using std::string;

const unsigned char XML_REQUEST_FIRST_BYTE = 5;
const unsigned char XML_RESPONSE_FIRST_BYTE = 6;

//TODO: Rename CXMLAcceptHandler (it's not really an xml accepter anymore is it?)

CXMLAcceptHandler::CXMLAcceptHandler() : CAcceptHandler( 1 )
{

}

CXMLAcceptHandler::~CXMLAcceptHandler()
{

}

bool CXMLAcceptHandler::HandleAccept( CTCPConnection::TPointer inpNewConnection )
{
	CReceiveHandler::TPointer oPointer = CPBRequestHandler::Create( inpNewConnection, CSyncroPBResponseFactory::Create() );
	inpNewConnection->AddRecvHandler( oPointer, 1 );
	//TODO: This is a fairly large bug: the xml request handler could easily intercept stuff before handshake.
	//		Not a priority, but fix this sometime (possibly just by replacing xml with protocol buffers)
	inpNewConnection->AddRecvHandler( CXMLRequestHandler::Create( inpNewConnection ) , 2 );
	inpNewConnection->StartRecv( 0 );
	return true;
}

CXMLRequestHandler::CXMLRequestHandler( CTCPConnection::TPointer inpConn ) : m_pConn( inpConn )
{
	m_fCloseConnection = false;
}

bool CXMLRequestHandler::CanHandleReceive( const TCharBuffer& inoBuffer )
{
	if( inoBuffer.nSize > 2 )
	{
		if( inoBuffer.aBuffer[0] == XML_REQUEST_FIRST_BYTE )
		{
			//TODO: Convert endianness
			unsigned int nExpectedSize = ( *( int* )( &inoBuffer.aBuffer[1] ) );
			nExpectedSize = kode::utils::FromJavaEndian( nExpectedSize );
			if( inoBuffer.nSize >= nExpectedSize )
				return true;
		}
	}
	return false;
}

bool CXMLRequestHandler::HandleReceive( const TCharBuffer& inoBuffer )
{

	unsigned char nExpectedSize = inoBuffer.aBuffer[1];
	const string sGetFolderList = "GET_FOLDER_LIST";
	const string sGetFolderContents = "GET_FOLDER_CONTENTS:";

	string sBuffer( inoBuffer.begin() + 1 + 4, inoBuffer.end() );
	if( sBuffer.compare( sGetFolderList ) == 0 )
	{
		//Requesting folder list
		if( m_oXMLBuilder.GetFolderXML() )
		{
			CXMLSendHandler::TPointer pSender = CXMLSendHandler::Create( m_pConn );
			pSender->CreateXML( CXMLOutput( m_oXMLBuilder ) );
			m_pConn->Send( pSender );
		}
		else
			throw std::runtime_error( "Failed to get folder xml\n" );
	}
	else if( sBuffer.substr( 0, sGetFolderContents.length() ).compare( sGetFolderContents ) == 0 )
	{
		if( m_oXMLBuilder.GetFolderContentsXML( boost::lexical_cast<int>( sBuffer.substr( sGetFolderContents.length(),  sBuffer.length() - sGetFolderContents.length() ) ) ) )
		{
			CXMLSendHandler::TPointer pSender = CXMLSendHandler::Create( m_pConn );
			pSender->CreateXML( CXMLOutput( m_oXMLBuilder ) );
			m_pConn->Send( pSender );
		}
	}
	return true;
}

CXMLSendHandler::CXMLSendHandler( CTCPConnection::TPointer inpConn ) : m_pConn( inpConn )
{
	m_aBuffer.push_back( XML_RESPONSE_FIRST_BYTE );
	//Add space for the legnth of the message, which will be added after XML
	m_aBuffer.push_back( 0 );
	m_aBuffer.push_back( 0 );
	m_aBuffer.push_back( 0 );
	m_aBuffer.push_back( 0 );
}

CXMLSendHandler::~CXMLSendHandler()
{

}

void CXMLSendHandler::SendDone( int innSent )
{
	m_pConn->StartRecv( 0 );
}

bool CXMLSendHandler::SendStarting()
{
	//Fill out the size in the buffer
	size_t nBufferSize = m_aBuffer.size();
	if( nBufferSize < 5 )
		return false;
	*(( int32_t* )&m_aBuffer[1] ) = kode::utils::ToJavaEndian<int32_t>(( int32_t )nBufferSize );
	return true;
}


};	//namespace syncro
