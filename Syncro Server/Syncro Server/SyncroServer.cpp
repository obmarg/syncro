
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

namespace syncro {

using namespace std;
using std::tr1::shared_ptr;

CSyncroServer::CSyncroServer() :
	m_oBroadcastThread( CBroadcastThread() ){

}

CSyncroServer::~CSyncroServer() {
}

bool 
CSyncroServer::Run() {
	using boost::asio::io_service;
	
	io_service oIO;
	
	CServerComms oComms(oIO);
	//TODO: Rename CXMLAcceptHandler (it's not really an xml accepter anymore is it?)
	CXMLAcceptHandler* pXMLAH = new CXMLAcceptHandler();
	oComms.AddAcceptHandler( boost::shared_ptr<CAcceptHandler>( (pXMLAH) ) );
	oIO.run();

	return true;
}

};		//namespace syncro