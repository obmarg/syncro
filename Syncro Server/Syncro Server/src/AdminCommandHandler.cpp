#include "AdminCommandHandler.h"
#include "AdminCommandManager.h"
#include "SyncroPBResponseFactory.h"
#include "AuthManager.h"
#include <libsyncro/protocol_buffers/admin.pb.h>
#include <libsyncro/packet_types.h>

namespace syncro {

class CAdminCommandResponse : public CBasePBResponse {
	friend class CAdminCommandHandler;
public:
	virtual std::vector<unsigned int> GetSubpacketSizes() {
		if( !m_pMessage )
			throw std::logic_error( "CAdminCommandResponse::GetSubpacketSizes called on uninitialised object" );
		std::vector<unsigned int> aSizes(1);
		aSizes[0] = m_pMessage->ByteSize();
		return aSizes;
	}
	virtual unsigned int GetSubpacketCount() { return 1; };

	virtual unsigned int GetPacketType();

	virtual void WriteSubpacket(int inSubpacketIndex,google::protobuf::io::ZeroCopyOutputStream& stream) {
		if( inSubpacketIndex != 0 )
			throw std::logic_error( "CAdminCommandResponse::WriteSubpacket called with imaginary subpacket index" );
		m_pMessage->SerializeToZeroCopyStream( &stream );
	}
protected:
	CAdminCommandResponse(bool fOK,int nErrorCode=0);

	boost::scoped_ptr< google::protobuf::MessageLite > m_pMessage;
};


CAdminCommandHandler::CAdminCommandHandler(TInputStreamList& inaInputStreams,  const CAuthToken& inUserAuth, CAdminCommandManager& commandManager ) {
	pb::GenericAdminCommand oCommand;
	oCommand.ParseFromZeroCopyStream( inaInputStreams[0] );
	if( !oCommand.has_command() )
		throw std::runtime_error( "Invalid command recieved by CAdminCommandHandler" );
	try {

		StringMap map;
		for( int paramNum=0; paramNum < oCommand.params_size(); ++paramNum )
		{
			const syncro::pb::AdminParameter& param = oCommand.params( paramNum );
			if( param.has_int_value() )
			{
				throw std::logic_error( "AdminCommandHandler passed int value parameter, but not implemented yet");
			}
			if( !param.has_string_value() )
			{
				throw std::runtime_error( "AdminCommandHandler needs to be passed a string value");
			}
			map.insert( std::make_pair( param.name(), param.string_value() ) );
		}

		commandManager.HandleCommand( oCommand.command(), map, inUserAuth );
		m_pResponse.reset( new CAdminCommandResponse( true ) );
	}
	catch( const admin_command_exception& ex ) {
		m_pResponse.reset( new CAdminCommandResponse( false, ex.GetErrorCode() ) );
	}
}

CAdminCommandHandler::~CAdminCommandHandler() {

}

CAdminCommandResponse::CAdminCommandResponse(bool fOK,int nErrorCode) {
	m_pMessage.reset( new pb::AdminAck() );
	pb::AdminAck* pMessage = dynamic_cast< pb::AdminAck* > ( m_pMessage.get() );
	pMessage->set_ok( fOK );
	if( !fOK )
		pMessage->set_error_code( nErrorCode );
}

unsigned int CAdminCommandResponse::GetPacketType() {
	return comms::packet_types::AdminAck;
}

}
