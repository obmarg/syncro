/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "HandshakeHandlers.h"
#include "SyncroPBResponseFactory.h"
#include "ResponseFunctions.h"
#include "UserSession.h"
#include "SyncroDB.h"
#include <libsyncro/comms.h>
#include <libsyncro/packet_types.h>
#include <kode/utils.h>
#include <kode/db/statement.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <vector>
#include <string>
#include <iterator>

namespace syncro {
namespace pbHandlers {

using std::vector;
using std::string;

static BasePBResponse::TPointer HandleHandshake(
	InputStreamListPtr inputStreams,
	server::UserSession& session
	)
{
	HandshakeRequest oRequest( *inputStreams );
	BasePBResponse::TPointer pResponse = oRequest.GetResponse();
	if( oRequest.HasAuthDetails() )
	{
		session.Authenticate( oRequest.GetUsername(), oRequest.GetPassword() );
	}
	else
	{
		session.DefaultAuth();
	}
	//if we've got this far and haven't thrown, we're authed
	return pResponse;
}

static BasePBResponse::TPointer HandleSalt(
	InputStreamListPtr inputStreams,
	server::UserSession& session
	)
{
	return CSaltResponse::Create( session.GetSalt() );
}

static const server::RegisterSessionResponse handshakeRegister(
	comms::packet_types::HandshakeRequest,
	&HandleHandshake
	);

static const server::RegisterSessionResponse saltRegister(
	comms::packet_types::SaltRequest,
	&HandleSalt
	);

HandshakeRequest::HandshakeRequest( 
	const InputStreamList& inaInputStreams 
	)
{
	pb::HandshakeRequest oRequest;
	if( inaInputStreams.size() == 1 )
	{
		if( !oRequest.ParseFromZeroCopyStream( inaInputStreams[0] ) )
			throw authentication_exception( "invalid andshakeRequest packet" );
		if( oRequest.magic().compare( comms::HANDSHAKE_REQUEST_MAGIC ) != 0 )
			throw authentication_exception( "invalid magic" );
		if( oRequest.has_client_ver_major() )
			m_nMajorVersion = oRequest.client_ver_major();
		else
			m_nMajorVersion = -1;
		if( oRequest.has_client_ver_minor() )
			m_nMinorVersion = oRequest.client_ver_minor();
		else
			m_nMinorVersion = 0;
		if( oRequest.has_username() && oRequest.has_password() )
		{
			m_fHasAuth = true;
			m_UserName = oRequest.username();
			m_Password = oRequest.password();
		}
		else
			m_fHasAuth = false;
	}
	else
	{
		throw authentication_exception( "Invalid number of protocol buffers passed to handshake" );
	}
}

BasePBResponse::TPointer HandshakeRequest::GetResponse()
{
	BasePBResponse::TPointer pResponse( new HandshakeResponse() );
	return pResponse;
}

HandshakeResponse::HandshakeResponse()
{
	m_oMessage.Clear();
	m_oMessage.set_magic( comms::HANDSHAKE_RESPONSE_MAGIC );
#ifdef _WIN32
	m_oMessage.set_windows( true );
#else
	m_oMessage.set_windows( false );
#endif

	kode::db::DatabasePtr oDB = SyncroDB::OpenDB();
	std::string sUUID;
	vector<unsigned char> aUUID( 16 );
	try
	{
		sUUID = oDB->runScalar<std::string>( "SELECT uuid FROM ServerID" );
		if( sUUID.length() != 16 )
			throw std::runtime_error( "Invalid legnth of UUID returned from database in CHandshakeResponse" );
	}
	catch( const std::out_of_range& )
	{
		//This probably means that the database call returned nothing (probably can happen for copying as well)
		sUUID = kode::utils::GenerateUUID();
		//TODO: Make the hostname customisable perhaps?
		std::string sHostname = boost::asio::ip::host_name();
		
		oDB->prepare( "INSERT INTO ServerID(uuid,servername) VALUES(?,?);" )->
			Bind( 1, sUUID )
			.Bind( 2, sHostname )
			.GetNextRow();

	}
	m_oMessage.set_uuid( sUUID );
}

uint32_t HandshakeResponse::GetSubpacketSize(uint32_t subpacket)
{
	assert( subpacket == 0 );
	return m_oMessage.ByteSize();
}

unsigned int HandshakeResponse::GetPacketType()
{
	return comms::packet_types::HandshakeResponse;
}

void HandshakeResponse::WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream )
{
	WriteMessage( m_oMessage, stream );
}

CSaltResponse::CSaltResponse( const std::string& salt )
{
	m_aSubpackets.resize( 1 );
	m_aSubpackets[0].reset( new TCharBuffer::TBuff() );
	pb::SaltResponse response;
	response.set_salt( salt );
	std::size_t size = response.ByteSize();
	m_aSubpackets[0]->resize( size );
	response.SerializeToArray( &m_aSubpackets[0]->at( 0 ), size );
}

unsigned int CSaltResponse::GetPacketType()
{
	return comms::packet_types::SaltResponse;
}

}	// namespace pbHandlers
}	// namespace syncro
