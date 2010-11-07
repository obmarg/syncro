#ifndef _ADMIN_COMMAND_MANAGER_H_
#define _ADMIN_COMMAND_MANAGER_H_

#include "common.h"
#include <boost/noncopyable.hpp>
#include <stdexcept>
#include <vector>

namespace syncro {

enum eAdminCommand {
	eAdminCommand_AddFolder,
	eAdminCommand_DelFolder,
	eAdminCommand_AddLocalFile,
	eAdminCommand_Total
};

class admin_command_exception : public std::runtime_error {
public:
	admin_command_exception( int nErrorCode ) : m_nErrorCode( nErrorCode ), runtime_error( "Admin Command Exception" ) {};

	int GetErrorCode() const { return m_nErrorCode; };
private:
	int m_nErrorCode;
};

class CAdminCommandManager : boost::noncopyable {
public:
	CAdminCommandManager();

	void HandleCommand( const std::string& sName, const std::string& sParam, const CAuthToken& insAuth ) const;

private:
	struct sAdminCommand {
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
};

};

#endif