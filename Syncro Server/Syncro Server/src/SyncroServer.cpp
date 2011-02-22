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

#include "SyncroServer.h"
#include "FolderMan.h"
#include "BroadcastThread.h"
#include "ServerComms.h"
#include "PBRequestHandler.h"
#include "SyncroPBResponseFactory.h"
#include "SyncroDB.h"
#include "libsyncro/comms.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace syncro
{

using namespace std;
using boost::shared_ptr;

SyncroServer::SyncroServer( unsigned int port ) :
	BaseAcceptHandler( 1 ),
	m_oBroadcastThread( CBroadcastThread() ),
	m_port( port )
{
	if( port == 0 )
		m_port = comms::SERVER_PORT;
	//Create a DB object.  to ensure the db file is created
	//if it doesn't already exist
	CSyncroDB::OpenDB();
}

SyncroServer::~SyncroServer()
{
}

bool SyncroServer::Run()
{
	using boost::asio::io_service;

	io_service oIO;

	ServerComms oComms( oIO, m_port );
	oComms.AddAcceptHandler( shared_from_this() );
	oIO.run();

	return true;
}

bool SyncroServer::HandleAccept( 
	CTCPConnection::TPointer inpNewConnection 
	)
{
	CReceiveHandler::TPointer recvHandler = 
		CPBRequestHandler::Create( 
			inpNewConnection, 
			CSyncroPBResponseFactory::Create() 
			);

	inpNewConnection->AddRecvHandler( recvHandler, 1 );
	inpNewConnection->StartRecv( 0 );
	return true;
}

};		//namespace syncro
