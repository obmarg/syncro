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

namespace syncro {

using std::vector;
using std::string;

const std::string CPBHandshakeRequest::m_sRecvString = "Hello Syncro?";

CPBHandshakeRequest::CPBHandshakeRequest(TInputStreamList& inaInputStreams) {
	pb::HandshakeRequest oRequest;
	if( inaInputStreams.size() == 1 ) {
		if( !oRequest.ParseFromZeroCopyStream( inaInputStreams[0] ) )
			throw authentication_exception( "invalid andshakeRequest packet" );
		if( oRequest.magic().compare( m_sRecvString ) != 0 )
			throw authentication_exception( "invalid magic" );
		if( oRequest.has_client_ver_major() )
			m_nMajorVersion = oRequest.client_ver_major();
		else 
			m_nMajorVersion = -1;
		if( oRequest.has_client_ver_minor() )
			m_nMinorVersion = oRequest.client_ver_minor();
		else
			m_nMinorVersion = 0;
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
	m_oMessage.set_magic( "Hey bitch!" );

	Database::TPointer oDB = CSyncroDB::OpenDB();
	std::string sUUID;
	vector<unsigned char> aUUID( 16 );
	try {
		sUUID = oDB->runScalar<std::string>("SELECT uuid FROM ServerID");
		if( sUUID.length() != 16 )
			throw std::runtime_error( "Invalid legnth of UUID returned from database in CHandshakeResponse" );
	}catch( const std::out_of_range& ) {
		//This probably means that the database call returned nothing (probably can happen for copying as well)
		boost::uuids::random_generator oGenerator;
		boost::uuids::uuid oUUID( oGenerator() );
		if( oUUID.size() != 16 )
			throw std::runtime_error( "Invalid legnth of UUID generated in CHandshakeResponse" );
		for( int n=0; n<oUUID.size(); n++ ) {
			aUUID[n] = oUUID.data[n];
		}
		sUUID = std::string( aUUID.begin(), aUUID.end() );
		//TODO: Make the hostname customisable perhaps?
		std::string sHostname = boost::asio::ip::host_name();
		oDB->run("INSERT INTO ServerID(uuid,servername) VALUES('" + sUUID + "','" + sHostname + "');");
	}
	m_oMessage.set_uuid( sUUID );
}

std::vector<unsigned int> CPBHandshakeResponse::GetSubpacketSizes() {
	std::vector<unsigned int> aSizes(1);
	aSizes[0] = m_oMessage.ByteSize();
	return aSizes;
}

unsigned int CPBHandshakeResponse::GetPacketType() {
	return eSyncroPBPacketTypes_HandshakeResponse;
}

void CPBHandshakeResponse::WriteSubpacket(int inSubpacketIndex,google::protobuf::io::ZeroCopyOutputStream& stream) {
	WriteMessage( m_oMessage, stream);
}


};		//namespace syncro
