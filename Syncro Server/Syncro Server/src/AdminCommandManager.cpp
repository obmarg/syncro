#include "AdminCommandManager.h"
#include "AuthManager.h"
#include "SyncroDB.h"
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace syncro {

class MissingParamException : public std::runtime_error
{
public:
	MissingParamException( const std::string& what )
		:runtime_error( what.c_str() )
	{

	}
	MissingParamException( const char* what )
		:runtime_error( what )
	{

	}
};

CAdminCommandManager::CAdminCommandManager() : m_aCommands( eAdminCommand_Total )
{
	m_aCommands[ eAdminCommand_AddFolder ].Set( "AddFolder", CAuthToken::AccessLevel_Admin );
	m_aCommands[ eAdminCommand_DelFolder ].Set( "DelFolder", CAuthToken::AccessLevel_Admin );
	m_aCommands[ eAdminCommand_AddLocalFile ].Set( "AddLocalFile", CAuthToken::AccessLevel_Normal );
	m_db = CSyncroDB::OpenDB();
	m_addFolder = m_db->prepare( "INSERT INTO Folders(Name,Path) VALUES (?,?);" );
	m_delFolder = m_db->prepare( "DELETE FROM Folders WHERE ID=?;" );
	//TODO: Set up m_addLocalFile;
}

void CAdminCommandManager::HandleCommand( const std::string& sName, const StringMap& params, const CAuthToken& insAuth ) {
	eAdminCommand command = FindCommand( sName );
	if( ( !insAuth.IsInitialised() ) || m_aCommands[ command ].AuthLevel > insAuth.GetAccessLevel() )
		throw admin_command_exception( -1 );
	
	try {
		switch( command ) {
		case eAdminCommand_AddFolder:
			AddFolder( params );
			break;
		case eAdminCommand_DelFolder:
			DelFolder( 
				boost::lexical_cast<unsigned int>( 
					 GetParam( params, "id" ) 
					 ) 
				);
			break;
		case eAdminCommand_AddLocalFile:
			AddLocalFile( GetParam( params, "filename") );
			break;
		default:
			throw admin_command_exception( -2 );
		}
	}
	catch( const MissingParamException& ex )
	{
		std::cout << "Admin command failed: " << sName << "\n";
		std::cout << "Missing parameter: " << ex.what() << "\n";
		//TODO: Throw admin command exception?
	}
}

eAdminCommand CAdminCommandManager::FindCommand( const std::string& name ) const {
	int nRV = eAdminCommand_AddFolder;
	BOOST_FOREACH( const sAdminCommand& command, m_aCommands ) {
		if( command.Name.compare( name ) == 0 )
			return static_cast<eAdminCommand>( nRV );
		++nRV;
	}
	throw admin_command_exception( -3 );
}

std::string CAdminCommandManager::GetParam( 
	const StringMap& params, 
	const std::string& name 
	) const
{
	StringMap::const_iterator it = 
		params.find( name );
	if( it == params.end() )
		throw MissingParamException(name);
	return it->second;
};

void CAdminCommandManager::AddFolder(
	const StringMap& params
	) 
{
	std::string folderPath( GetParam( params, "path" ) );

	boost::filesystem::path folder( folderPath );
	if( boost::filesystem::exists( folder ) ) {
		kode::db::AutoReset ar( m_addFolder );
		m_addFolder->Bind( 1, GetParam( params, "name") );
		m_addFolder->Bind( 2, folderPath );
		m_addFolder->GetNextRow();
	} else {
		//TODO: Add symbolic constants for these exceptions...
		throw admin_command_exception( -4 );
	}
}

void CAdminCommandManager::DelFolder(unsigned int Id) {
	kode::db::AutoReset ar( m_delFolder );
	m_delFolder->Bind( 1, Id );
	m_delFolder->GetNextRow();
}

void CAdminCommandManager::AddLocalFile( const std::string& path) {
	
}

};		// namespace syncro
