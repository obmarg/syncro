
#include "XMLBuilder.h"
#include "FolderMan.h"
#include "tinyxml.h"
#include <fstream>
#include <iterator>
#include <vector>
#include <string>


using std::vector;
using std::string;

CXMLBuilder::CXMLBuilder() {

}

CXMLBuilder::~CXMLBuilder() {

}

void CXMLBuilder::SaveDocAsFile(std::string insFile) {
	m_oXML.SaveFile( insFile.c_str() );
}

bool CXMLBuilder::GetFolderXML() {
	CFolderMan oFolderMan("C:\\SyncFiles\\");

	m_oXML = TiXmlDocument();
	TiXmlDeclaration * oDecl = new TiXmlDeclaration( "1.0", "", "" );
	m_oXML.LinkEndChild( oDecl );
	TiXmlElement *pFoldersElement = new TiXmlElement( "Folders" );
	m_oXML.LinkEndChild( pFoldersElement );

	const vector< string >& oFolders = oFolderMan.GetFolders();
	TiXmlElement *pFolder;
	for( vector<string>::const_iterator oFolder = oFolders.begin(); oFolder != oFolders.end(); oFolder++ ) {
		pFolder = new TiXmlElement( "Folder" );
		TiXmlText* pText = new TiXmlText( TiXmlText( oFolder->c_str() ) );
		pFolder->LinkEndChild( pText );
		pFoldersElement->LinkEndChild( pFolder );
	}
	return true;
}

bool CXMLBuilder::GetFolderContentsXML() {
	using boost::shared_ptr;
	
	CFolderMan oFolderMan("C:\\SyncFiles\\");
	
	m_oXML = TiXmlDocument();

	shared_ptr<CFolder> pCurrentFolder = oFolderMan.GetFolder( 0 );

	TiXmlDocument oDoc;
	TiXmlDeclaration * oDecl = new TiXmlDeclaration( "1.0", "", "" );
	oDoc.LinkEndChild( oDecl );
	TiXmlElement *pFoldersElement = new TiXmlElement( "RootFolder" );
	oDoc.LinkEndChild( pFoldersElement );
	
	ProcessFolder( *pFoldersElement, pCurrentFolder );
	return true;
}

void CXMLBuilder::ProcessFolder( TiXmlElement& inoParentElement, boost::shared_ptr<class CFolder> inoFolder ) {
	using boost::shared_ptr;

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

void CXMLOutput::operator()(TCharBuffer::TBuff& inoBuffer) {
	using namespace std;

	m_oBuilder.SaveDocAsFile("temp.xml");
	
	ifstream oStream( "temp.xml", ios::in || ios::binary );
	if( oStream.is_open() ) {
		oStream.seekg( 0, ios::end );
		size_t nSize = (size_t)oStream.tellg();
		size_t nBuffStart = inoBuffer.size();
		inoBuffer.resize( nSize + nBuffStart );

		oStream.seekg( 0, ios::beg );
		oStream.read( (char*)&inoBuffer[nBuffStart], nSize );
		nSize = inoBuffer.size();
		//Remove trailing nulls
		while( inoBuffer[nSize-1] == 0 ) {
			--nSize;
		}
		inoBuffer.resize( nSize );
	}
	return;
}