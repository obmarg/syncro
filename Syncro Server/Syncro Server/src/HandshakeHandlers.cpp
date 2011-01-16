//This define is needed due to some of the uuid stuff before.  doesn't seem to like it for some reason
#define _SCL_SECURE_NO_WARNINGS

#include "HandshakeHandlers.h"
#include "XMLHandlers.h"
#include "PBRequestHandler.h"
#include "SyncroPBResponseFactory.h"

#include <vector>
#include <string>
#include <iterator>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "SyncroDB.h"

#include <libsyncro/comms.h>
#include <kode/utils.h>

namespace syncro {

using std::vector;
using std::string;

CPBHandshakeRequest::CPBHandshakeRequest(TInputStreamList& inaInputStreams) {
	pb::HandshakeRequest oRequest;
	if( inaInputStreams.size() == 1 ) {
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
		} else
			m_fHasAuth = false;
	} else {
		throw authentication_exception("Invalid number of protocol buffers passed to handshake");
	}
}

CBasePBResponse::TPointer CPBHandshakeRequest::GetResponse() {
	CBasePBResponse::TPointer pResponse( new CPBHandshakeResponse() );
	return pResponse;
}

CPBHandshakeResponse::CPBHandshakeResponse() {
	m_oMessage.Clear();
	//TODO: move this string to a static const or something
	m_oMessage.set_magic( comms::HANDSHAKE_RESPONSE_MAGIC );

	kode::db::Database::TPointer oDB = CSyncroDB::OpenDB();
	std::string sUUID;
	vector<unsigned char> aUUID( 16 );
	try {
		sUUID = oDB->runScalar<std::string>("SELECT uuid FROM ServerID");
		if( sUUID.length() != 16 )
			throw std::runtime_error( "Invalid legnth of UUID returned from database in CHandshakeResponse" );
	}catch( const std::out_of_range& ) {
		//This probably means that the database call returned nothing (probably can happen for copying as well)
		sUUID = kode::utils::GenerateUUID();
		//TODO: Make the hostname customisable perhaps?
		std::string sHostname = boost::asio::ip::host_name();
		//TODO: this can exception if the uuid contains the wrong characters.  fix it (presumably by adding statements & binding)
		//oDB->run("INSERT INTO ServerID(uuid,servername) VALUES('" + sUUID + "','" + sHostname + "');");
		//TODO: Make sure this works
		oDB->prepare("INSERT INTO ServerID(uuid,servername) VALUES(?,?);")->
			Bind(1,sUUID)
			.Bind(2,sHostname)
			.GetNextRow();
		
	}
	m_oMessage.set_uuid( sUUID );
}

std::vector<unsigned int> CPBHandshakeResponse::GetSubpacketSizes() {
	std::vector<unsigned int> aSizes(1);
	aSizes[0] = m_oMessage.ByteSize();
	return aSizes;
}

unsigned int CPBHandshakeResponse::GetPacketType() {
	return comms::packet_types::HandshakeResponse;
}

void CPBHandshakeResponse::WriteSubpacket(int inSubpacketIndex,google::protobuf::io::ZeroCopyOutputStream& stream) {
	WriteMessage( m_oMessage, stream);
}

CSaltResponse::CSaltResponse(std::string salt)
{
	m_aSubpackets.resize( 1 );
	m_aSubpackets[0].reset( new TCharBuffer::TBuff() );
	pb::SaltResponse response;
	response.set_salt( salt );
	std::size_t size = response.ByteSize();
	m_aSubpackets[0]->resize( size );
	response.SerializeToArray( &m_aSubpackets[0]->at(0), size );
}

unsigned int CSaltResponse::GetPacketType()
{
	return comms::packet_types::SaltResponse;
}

};		//namespace syncro
