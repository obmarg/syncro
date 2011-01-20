#ifndef _ADMIN_COMMAND_HANDLER_H_
#define _ADMIN_COMMAND_HANDLER_H_

#include "common.h"
#include "BasePBResponse.h"
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <google/protobuf/message_lite.h>

namespace syncro
{

class CAuthToken;
class CAdminCommandManager;

class CAdminCommandHandler
{
public:
	CAdminCommandHandler( TInputStreamList& inaInputStreams, const CAuthToken& inUserAuth, CAdminCommandManager& commandManager );
	~CAdminCommandHandler();

	CBasePBResponse::TPointer GetResponse()
	{
		return m_pResponse;
	}
private:
	CBasePBResponse::TPointer m_pResponse;
};

};

#endif