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

class CAdminCommandManager : boost::noncopyable
{
public:
	CAdminCommandManager();

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
