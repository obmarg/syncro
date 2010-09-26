
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
	return true;
}

void CXMLOutput::operator()(TCharBuffer::TBuff& inoBuffer) {
	using namespace std;

	m_oBuilder.SaveDocAsFile("temp.xml");
	
	ifstream oStream( "temp.xml", ios::in );
	copy( istream_iterator< TCharBuffer::TChar >( oStream ), istream_iterator< TCharBuffer::TChar >(), back_insert_iterator< TCharBuffer::TBuff >( inoBuffer ) );
	return;
}