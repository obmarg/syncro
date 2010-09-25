
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
	
	TiXmlDocument oDoc;
	TiXmlDeclaration * oDecl = new TiXmlDeclaration( "1.0", "", "" );
	oDoc.LinkEndChild( oDecl );
	TiXmlElement *pFoldersElement = new TiXmlElement( "Folders" );
	oDoc.LinkEndChild( pFoldersElement );

	const vector< string >& oFolders = m_oFolderMan.GetFolders();
	TiXmlElement *pFolder;
	for( vector<string>::const_iterator oFolder = oFolders.begin(); oFolder != oFolders.end(); oFolder++ ) {
		pFolder = new TiXmlElement( "Folder" );
		TiXmlText* pText = new TiXmlText( TiXmlText( oFolder->c_str() ) );
		pFolder->LinkEndChild( pText );
		pFoldersElement->LinkEndChild( pFolder );
	}

	return XmlToString(oDoc);
}

char *
CSyncroServer::GetFolderContentsXML() {
	using std::tr1::shared_ptr;
	shared_ptr<CFolder> pCurrentFolder = m_oFolderMan.GetFolder( boost::lexical_cast<int>( m_oHTTP.FindValue( "folderid" ) ) );

	TiXmlDocument oDoc;
	TiXmlDeclaration * oDecl = new TiXmlDeclaration( "1.0", "", "" );
	oDoc.LinkEndChild( oDecl );
	TiXmlElement *pFoldersElement = new TiXmlElement( "RootFolder" );
	oDoc.LinkEndChild( pFoldersElement );
	
	ProcessFolder( *pFoldersElement, pCurrentFolder );
	
	return XmlToString( oDoc );

}

char* CSyncroServer::XmlToString( TiXmlDocument& inoDoc ) {
	inoDoc.SaveFile( "temp.xml" );
	
	std::ifstream oStream( "temp.xml", ios::in );
	m_oOutputBuff.clear();
	copy( istream_iterator< char >( oStream ), istream_iterator< char >(), back_insert_iterator< vector<char> >( m_oOutputBuff ) );
	m_oOutputBuff.push_back( '\0' );
	return &m_oOutputBuff[0];
}

void
CSyncroServer::ProcessFolder( TiXmlElement& inoParentElement, shared_ptr<class CFolder> inoFolder ) {
	TiXmlElement* pThisElement = new TiXmlElement( "Folder" );
	pThisElement->SetAttribute( " name", inoFolder->GetName().c_str() );
	inoParentElement.LinkEndChild( pThisElement );
	vector< string > oFiles = inoFolder->GetFiles();
	for( vector< string >::iterator oFile = oFiles.begin(); oFile != oFiles.end(); oFile++ ) {
		TiXmlElement* pFileElement = new TiXmlElement( "File" );
		TiXmlText *pText = new TiXmlText( oFile->c_str() );
		pFileElement->LinkEndChild( pText );
		pThisElement->LinkEndChild( pFileElement );
	}
	vector< shared_ptr<CFolder> > oChildren = inoFolder->GetChildren();
	for( vector< shared_ptr<CFolder> >::iterator oChild = oChildren.begin(); oChild != oChildren.end(); oChild++ ) {
		ProcessFolder( (*pThisElement), (*oChild) );
	}
}