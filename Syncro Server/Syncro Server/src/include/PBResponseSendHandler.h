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

#ifndef _PB_RESPONSE_SEND_HANDLER_H_
#define _PB_RESPONSE_SEND_HANDLER_H_

#include "common.h"
#include "ServerComms.h"
#include "BasePBResponse.h"

namespace syncro
{

class PBResponseSendHandler : public SendHandler
{
public:
	virtual ~PBResponseSendHandler();
	virtual bool SendStarting();	//Want to override this definetly (i think)

	static SendHandler::TPointer Create( TCPConnection& connection )
	{
		return SendHandler::TPointer( new PBResponseSendHandler( connection ) );
	}

	void SetPBResponse( BasePBResponse::TPointer inpResponse )
	{
		m_pResponse = inpResponse;
	}

	virtual void SendDone( int innSent );
protected:

	TCPConnection& m_connection;

	BasePBResponse::TPointer m_pResponse;

	PBResponseSendHandler( TCPConnection& connection );
};

} //namespace syncro;

#endif
