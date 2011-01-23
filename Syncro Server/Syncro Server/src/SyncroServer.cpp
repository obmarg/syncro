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
#include <iostream>
#include <boost/bind.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include "tinyxml.h"
#include "FolderMan.h"
#include <memory>
#include <boost/lexical_cast.hpp>
#include "BroadcastThread.h"
#include "ServerComms.h"
#include <boost/asio.hpp>
#include "XMLHandlers.h"
#include "HandshakeHandlers.h"
#include "SyncroPBResponseFactory.h"
#include "SyncroDB.h"

namespace syncro
{

using namespace std;
using boost::shared_ptr;

CSyncroServer::CSyncroServer() :
	m_oBroadcastThread( CBroadcastThread() )
{
	//Create a DB object.  to ensure the db file is created
	//if it doesn't already exist
	CSyncroDB::OpenDB();
}

CSyncroServer::~CSyncroServer()
{
}

bool
CSyncroServer::Run()
{
	using boost::asio::io_service;

	io_service oIO;

	CServerComms oComms( oIO );
	//TODO: Rename CXMLAcceptHandler (it's not really an xml accepter anymore is it?)
	CXMLAcceptHandler* pXMLAH = new CXMLAcceptHandler();
	oComms.AddAcceptHandler( boost::shared_ptr<CAcceptHandler>(( pXMLAH ) ) );
	oIO.run();

	return true;
}

};		//namespace syncro
