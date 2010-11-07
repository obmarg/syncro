#include "AdminCommandManager.h"
#include "AuthManager.h"
#include <boost/foreach.hpp>

namespace syncro {

CAdminCommandManager::CAdminCommandManager() : m_aCommands( eAdminCommand_Total )
{
	m_aCommands[ eAdminCommand_AddFolder ].Set( "AddFolder", CAuthToken::AccessLevel_Admin );
	m_aCommands[ eAdminCommand_DelFolder ].Set( "DelFolder", CAuthToken::AccessLevel_Admin );
	m_aCommands[ eAdminCommand_AddLocalFile ].Set( "AddLocalFile", CAuthToken::AccessLevel_Normal );
}

void CAdminCommandManager::HandleCommand( const std::string& sName, const std::string& sParam, const CAuthToken& insAuth ) const {
	eAdminCommand command = FindCommand( sName );
	if( ( !insAuth.IsInitialised() ) || m_aCommands[ command ].AuthLevel > insAuth.GetAccessLevel() )
		throw new admin_command_exception( -1 );
	switch( command ) {
	case eAdminCommand_AddFolder:
		//TODO: add a folder
		break;
	case eAdminCommand_DelFolder:
		//TODO: delete a folder
		break;
	case eAdminCommand_AddLocalFile:
		//TODO: add a local file
		break;
	default:
		throw new admin_command_exception( -2 );
	}
}

eAdminCommand CAdminCommandManager::FindCommand( const std::string& name ) const {
	int nRV = eAdminCommand_AddFolder;
	BOOST_FOREACH( const sAdminCommand& command, m_aCommands ) {
		if( command.Name.compare( name ) == 0 )
			return static_cast<eAdminCommand>( nRV );
		++nRV;
	}
	throw new admin_command_exception( -3 );
}

};		// namespace syncro
