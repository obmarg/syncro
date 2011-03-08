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

#ifndef _ADMIN_COMMAND_HANDLER_H_
#define _ADMIN_COMMAND_HANDLER_H_

#include "common.h"
#include "BasePBResponse.h"
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <google/protobuf/message_lite.h>

namespace syncro {

class CAuthToken;
class AdminCommandManager;

namespace pbHandlers {

class AdminCommand
{
public:
	AdminCommand( 
		const InputStreamList& inaInputStreams, 
		const CAuthToken& inUserAuth, 
		AdminCommandManager& commandManager 
		);
	~AdminCommand();

	BasePBResponse::TPointer GetResponse()
	{
		return m_pResponse;
	}
private:
	BasePBResponse::TPointer m_pResponse;
};

}	// namespace pbHandlers
}	// namespace syncro

#endif