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

#ifndef _ADMIN_COMMAND_MANAGER_H_
#define _ADMIN_COMMAND_MANAGER_H_

#include "common.h"
#include <kode/db/database.h>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <stdexcept>
#include <vector>

namespace syncro
{

class admin_command_exception : public std::runtime_error
{
public:
	admin_command_exception( int nErrorCode ) :
		runtime_error( "Admin Command Exception" ),
		m_nErrorCode( nErrorCode )
	{};

	int GetErrorCode() const
	{
		return m_nErrorCode;
	};
private:
	int m_nErrorCode;
};

class AdminCommandManager : boost::noncopyable
{
public:
	AdminCommandManager();

	void HandleCommand( 
		const std::string& sName, 
		const StringMap& params, 
		const AuthToken& insAuth 
		);

private:
	typedef boost::function< void (const StringMap& params) >
		CommandCallback;

	struct AdminCommand
	{
		void Set( 
			std::string inName, 
			AuthToken::AccessLevel inAuthLevel,
			const CommandCallback& callback
			)
		{
			Name = inName;
			AuthLevel = inAuthLevel;
			Callback = callback;
		};
		std::string Name;
		AuthToken::AccessLevel AuthLevel;
		CommandCallback Callback;
	};
	std::vector<AdminCommand> m_aCommands;

	const AdminCommand& FindCommand( const std::string& sName ) const;

	std::string GetParam( const StringMap& params, const std::string& name ) const;

	void AddFolder( const StringMap& params );
	void DelFolder( const StringMap& params );
	void AddLocalFile( const StringMap& params );
	void AddUser( const StringMap& params );
	void DelUser( const StringMap& params );
	void ChangePassword( const StringMap& params );
	void ChangeUserPassword( const StringMap& params );

	void CreateChangePasswordStatement();

	kode::db::DatabasePtr m_db;
	kode::db::StatementPtr m_addFolder;
	kode::db::StatementPtr m_delFolder;
	kode::db::StatementPtr m_addLocalFile;
	kode::db::StatementPtr m_addUser;
	kode::db::StatementPtr m_delUser;
	kode::db::StatementPtr m_changePassword;
};

};

#endif
