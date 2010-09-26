
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

using namespace std;
using std::tr1::shared_ptr;

CSyncroServer::CSyncroServer() :
	m_oHTTP("0.0.0.0", "9995", "C:\\Web\\"), m_oFolderMan( "C:\\SyncFiles\\" ),
	m_oBroadcastThread( CBroadcastThread() )
{
		m_oHTTP.RegisterIncludeCode( "hello", boost::bind( &CSyncroServer::GetFolderXML, this ) );
		m_oHTTP.RegisterActionCode( "foldercontents", boost::bind( &CSyncroServer::GetFolderContentsXML, this ) );
}

CSyncroServer::~CSyncroServer() {
}

bool 
CSyncroServer::Run() {
	//m_oHTTP.Run();
	using boost::asio::io_service;
	io_service oIO;
	
	CServerComms oComms(oIO);
	CXMLAcceptHandler* pXMLAH = new CXMLAcceptHandler();
	oComms.AddAcceptHandler( boost::shared_ptr<CAcceptHandler>( (pXMLAH) ) );
	oIO.run();

	return true;
}

char* 
CSyncroServer::GetFolderXML() {

	return "";
}

char *
CSyncroServer::GetFolderContentsXML() {
	return "";
}

char* CSyncroServer::XmlToString( TiXmlDocument& inoDoc ) {
	return NULL;
}

void
CSyncroServer::ProcessFolder( TiXmlElement& inoParentElement, shared_ptr<class CFolder> inoFolder ) {

}