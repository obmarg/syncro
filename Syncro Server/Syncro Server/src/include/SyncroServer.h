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

#ifndef _SYNCRO_SERVER_H_
#define _SYNCRO_SERVER_H_

#include <boost/thread.hpp>
#include "ServerComms.h"
#include <boost/enable_shared_from_this.hpp>

namespace syncro
{

class SyncroServer : 
	public BaseAcceptHandler, 
	public boost::enable_shared_from_this<SyncroServer>
{
public:
	SyncroServer( unsigned int port=0 );
	~SyncroServer();

	bool Run();

	virtual bool HandleAccept( CTCPConnection::TPointer inpNewConnection );
private:
	boost::thread m_oBroadcastThread;
	unsigned int m_port;
};

};		//namespace syncro

#endif
