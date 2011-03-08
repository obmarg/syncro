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
#include <kode/Database.h>
#include <boost/noncopyable.hpp>
#include <stdexcept>
#include <vector>

namespace syncro
{

enum eAdminCommand
{
	eAdminCommand_AddFolder,
	eAdminCommand_DelFolder,
	eAdminCommand_AddLocalFile,
	eAdminCommand_Total
};

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

	void HandleCommand( const std::string& sName, const StringMap& params, const CAuthToken& insAuth );

private:
	struct sAdminCommand
	{
		void Set( std::string inName, CAuthToken::AccessLevel inAuthLevel )
		{
			Name = inName;
			AuthLevel = inAuthLevel;
		};
		std::string Name;
		CAuthToken::AccessLevel AuthLevel;
	};
	std::vector<sAdminCommand> m_aCommands;

	eAdminCommand FindCommand( const std::string& sName ) const;

	std::string GetParam( const StringMap& params, const std::string& name ) const;

	void AddFolder( const StringMap& params );
	void DelFolder( unsigned int Id );
	void AddLocalFile( const std::string& path );

	kode::db::DatabasePtr m_db;
	kode::db::StatementPtr m_addFolder;
	kode::db::StatementPtr m_delFolder;
	kode::db::StatementPtr m_addLocalFile;
};

};

#endif
