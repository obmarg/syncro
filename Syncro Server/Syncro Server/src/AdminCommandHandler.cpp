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

#include "AdminCommandHandler.h"
#include "AdminCommandManager.h"
#include "SyncroPBResponseFactory.h"
#include "AuthManager.h"
#include <libsyncro/protocol_buffers/admin.pb.h>
#include <libsyncro/packet_types.h>

namespace syncro {
namespace pbHandlers {

class AdminCommandHandler 
{
public:
	BasePBResponse::TPointer operator()(
		InputStreamListPtr inputStreams,
		server::UserSession& session
		)
	{
		//TODO: Would be good to sort out the reference/pointer situation
		//		sometime
		AdminCommand oHandler( 
			( *inputStreams ), 
			session.GetAuthToken(), 
			( *session.GetAdminCommandMan() )
			);
		return oHandler.GetResponse();
	}
};

static const server::RegisterSessionResponse registerAdminCommandHandler(
	comms::packet_types::AdminGenericCommand,
	AdminCommandHandler()
	);

class AdminCommandResponse : public BasePBResponse
{
	friend class AdminCommand;
public:
	virtual uint32_t GetSubpacketSize(uint32_t subpacket)
	{
		if( !m_pMessage )
			throw std::logic_error( "CAdminCommandResponse::GetSubpacketSizes called on uninitialised object" );
		assert( subpacket == 0 );
		return m_pMessage->ByteSize();
	}
	virtual unsigned int GetSubpacketCount()
	{
		return 1;
	};

	virtual unsigned int GetPacketType();

	virtual void WriteSubpacket( int inSubpacketIndex, google::protobuf::io::ZeroCopyOutputStream& stream )
	{
		if( inSubpacketIndex != 0 )
			throw std::logic_error( "CAdminCommandResponse::WriteSubpacket called with imaginary subpacket index" );
		m_pMessage->SerializeToZeroCopyStream( &stream );
	}
protected:
	AdminCommandResponse( bool fOK, int nErrorCode = 0 );

	boost::scoped_ptr< google::protobuf::MessageLite > m_pMessage;
};


AdminCommand::AdminCommand( 
	const InputStreamList& inaInputStreams,  
	const AuthToken& inUserAuth, 
	AdminCommandManager& commandManager )
{
	pb::GenericAdminCommand oCommand;
	oCommand.ParseFromZeroCopyStream( inaInputStreams[0] );
	if( !oCommand.has_command() )
		throw std::runtime_error( "Invalid command recieved by CAdminCommandHandler" );
	try
	{

		StringMap map;
		for( int paramNum = 0; paramNum < oCommand.params_size(); ++paramNum )
		{
			const syncro::pb::AdminParameter& param = oCommand.params( paramNum );
			if( param.has_int_value() )
			{
				throw std::logic_error( "AdminCommandHandler passed int value parameter, but not implemented yet" );
			}
			if( !param.has_string_value() )
			{
				throw std::runtime_error( "AdminCommandHandler needs to be passed a string value" );
			}
			map.insert( std::make_pair( param.name(), param.string_value() ) );
		}

		commandManager.HandleCommand( oCommand.command(), map, inUserAuth );
		m_pResponse.reset( new AdminCommandResponse( true ) );
	}
	catch( const admin_command_exception& ex )
	{
		m_pResponse.reset( new AdminCommandResponse( false, ex.GetErrorCode() ) );
	}
}

AdminCommand::~AdminCommand()
{

}

AdminCommandResponse::AdminCommandResponse( bool fOK, int nErrorCode )
{
	m_pMessage.reset( new pb::AdminAck() );
	pb::AdminAck* pMessage = dynamic_cast< pb::AdminAck* >( m_pMessage.get() );
	pMessage->set_ok( fOK );
	if( !fOK )
		pMessage->set_error_code( nErrorCode );
}

unsigned int AdminCommandResponse::GetPacketType()
{
	return comms::packet_types::AdminAck;
}

}	// namespace pbHandlers
}	// namespace syncro