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

#include "AdminCommandManager.h"
#include "AuthManager.h"
#include "SyncroDB.h"
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <iostream>

namespace syncro
{

enum AdminCommandId
{
	AddFolderCommandId,
	DelFolderCommandId,
	AddLocalFileCommandId,
	AddUserCommandId,
	DelUserCommandId,
	ChangePasswordCommandId,
	ChangeUserPasswordCommandId,
	AdminCommandIdTotal
};

class MissingParamException : public std::runtime_error
{
public:
	MissingParamException( const std::string& what )
		: runtime_error( what.c_str() )
	{

	}
	MissingParamException( const char* what )
		: runtime_error( what )
	{

	}
};

AdminCommandManager::AdminCommandManager() : m_aCommands( AdminCommandIdTotal )
{
	m_aCommands[ AddFolderCommandId ].Set( 
									"AddFolder", 
									AuthToken::AccessLevel_Admin,
									boost::bind(
										&AdminCommandManager::AddFolder,
										this, _1
										)
									);

	m_aCommands[ DelFolderCommandId ].Set( 
									"DelFolder", 
									AuthToken::AccessLevel_Admin,
									boost::bind(
										&AdminCommandManager::DelFolder,
										this, _1
										)
									);
	m_aCommands[ AddLocalFileCommandId ].Set( 
									"AddLocalFile", 
									AuthToken::AccessLevel_Normal, 
									boost::bind(
										&AdminCommandManager::AddLocalFile,
										this, _1
										)
									);
	m_aCommands[ AddUserCommandId ].Set(
									"AddUser",
									AuthToken::AccessLevel_Admin,
									boost::bind(
										&AdminCommandManager::AddUser,
										this, _1
										)
									);
	m_aCommands[ DelUserCommandId ].Set(
									"DelUser",
									AuthToken::AccessLevel_Admin,
									boost::bind(
										&AdminCommandManager::DelUser,
										this, _1
										)
									);
	m_aCommands[ ChangePasswordCommandId ].Set(
									"ChangePassword",
									AuthToken::AccessLevel_Normal,
									boost::bind(
										&AdminCommandManager::ChangePassword,
										this, _1
										)
									);
	m_aCommands[ ChangeUserPasswordCommandId ].Set(
									"ChangeUserPassword",
									AuthToken::AccessLevel_Admin,
									boost::bind(
										&AdminCommandManager::ChangeUserPassword,
										this, _1
										)
									);
	m_db = SyncroDB::OpenDB();
	//TODO: Set up m_addLocalFile;
}

void AdminCommandManager::HandleCommand( 
	const std::string& sName, 
	const StringMap& params, 
	const AuthToken& insAuth 
	)
{
	try {
		const AdminCommand& command = FindCommand( sName );
		if(
			( !insAuth.IsInitialised() ) || 
			command.AuthLevel > insAuth.GetAccessLevel() 
			)
		{
			throw admin_command_exception( -1 );
		}
		command.Callback( params );
	}
	catch( const MissingParamException& ex )
	{
		std::cout << "Admin command failed: " << sName << "\n";
		std::cout << "Missing parameter: " << ex.what() << "\n";
		//TODO: Throw admin command exception?
	}
}

const AdminCommandManager::AdminCommand& 
AdminCommandManager::FindCommand( 
	const std::string& name 
	) const
{
	BOOST_FOREACH( const AdminCommand & command, m_aCommands )
	{
		if( command.Name.compare( name ) == 0 )
			return command;
	}
	throw admin_command_exception( -3 );
}

std::string AdminCommandManager::GetParam(
    const StringMap& params,
    const std::string& name
) const
{
	StringMap::const_iterator it =
	    params.find( name );
	if( it == params.end() )
		throw MissingParamException( name );
	return it->second;
};

void AdminCommandManager::AddFolder(
    const StringMap& params
)
{
	std::string folderPath( GetParam( params, "path" ) );

	boost::filesystem::path folder( folderPath );
	if( boost::filesystem::exists( folder ) )
	{
		if( !m_addFolder )
		{
			m_addFolder = m_db->prepare( 
				"INSERT INTO Folders(Name,Path) VALUES (?,?);" 
				);
		}
		kode::db::AutoReset ar( m_addFolder );
		m_addFolder->Bind( 1, GetParam( params, "name" ) );
		m_addFolder->Bind( 2, folderPath );
		m_addFolder->GetNextRow();
	}
	else
	{
		//TODO: Add symbolic constants for these exceptions...
		throw admin_command_exception( -4 );
	}
}

void AdminCommandManager::DelFolder( const StringMap& params )
{
	if( !m_delFolder )
	{
		m_delFolder = m_db->prepare( "DELETE FROM Folders WHERE ID=?;" );
	}
	kode::db::AutoReset ar( m_delFolder );
	m_delFolder->Bind( 
		1, 
		boost::lexical_cast<int>( GetParam( params, "id" ) ) 
		);
	m_delFolder->GetNextRow();
}

void AdminCommandManager::AddLocalFile( const StringMap& params )
{
	std::string filename = GetParam( params, "filename" );
}

void AdminCommandManager::AddUser( const StringMap& params )
{
	
}

void AdminCommandManager::DelUser( const StringMap& params )
{

}

void AdminCommandManager::CreateChangePasswordStatement()
{
	if( !m_changePassword )
	{
		//TODO: Fill this in
		//m_changePassword = m_db->prepare(
		//	""
		//	)
	}
}

void AdminCommandManager::ChangePassword( const StringMap& params )
{
	CreateChangePasswordStatement();

}

void AdminCommandManager::ChangeUserPassword( const StringMap& params )
{
	CreateChangePasswordStatement();

}

};		// namespace syncro
