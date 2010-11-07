
#include "XMLBuilder.h"
#include "FolderMan.h"
#include "tinyxml.h"
#include <fstream>
#include <iterator>
#include <vector>
#include <string>

#include "SyncroDB.h"

namespace syncro {

using std::vector;
using std::string;

using namespace db;

CXMLBuilder::CXMLBuilder() {

}

CXMLBuilder::~CXMLBuilder() {

}

void CXMLBuilder::SaveDocAsFile(std::string insFile) {
	m_oXML.SaveFile( insFile.c_str() );
}

bool CXMLBuilder::GetFolderXML() {

	//TODO: make the database loading better, do it somewhere central probably?
	Database::TPointer oDB = CSyncroDB::OpenDB( );
	CFolderMan oFolderMan( oDB );

	m_oXML = TiXmlDocument();
	TiXmlDeclaration * oDecl = new TiXmlDeclaration( "1.0", "", "" );
	m_oXML.LinkEndChild( oDecl );
	TiXmlElement *pFoldersElement = new TiXmlElement( "Folders" );
	m_oXML.LinkEndChild( pFoldersElement );

	const CFolderMan::TFolderInfoList& oFolders = oFolderMan.GetFoldersInfo();
	TiXmlElement *pFolder;
	for( CFolderMan::TFolderInfoList::const_iterator oFolder = oFolders.begin(); oFolder != oFolders.end(); oFolder++ ) {
		pFolder = new TiXmlElement( "Folder" );
		pFolder->SetAttribute("ID", oFolder->nFolderID );
		TiXmlText* pText = new TiXmlText( TiXmlText( oFolder->sFolderName.c_str() ) );
		pFolder->LinkEndChild( pText );
		pFoldersElement->LinkEndChild( pFolder );
	}
	return true;
}

bool CXMLBuilder::GetFolderContentsXML(int innFolderID) {
	using boost::shared_ptr;
	
	Database::TPointer oDB = CSyncroDB::OpenDB( );
	CFolderMan oFolderMan( oDB );
	
	m_oXML = TiXmlDocument();

	shared_ptr<CFolder> pCurrentFolder = oFolderMan.GetFolder( innFolderID );

	TiXmlDeclaration * oDecl = new TiXmlDeclaration( "1.0", "", "" );
	m_oXML.LinkEndChild( oDecl );
	TiXmlElement *pFoldersElement = new TiXmlElement( "RootFolder" );
	m_oXML.LinkEndChild( pFoldersElement );
	
	ProcessFolder( *pFoldersElement, pCurrentFolder, true );
	return true;
}

void CXMLBuilder::ProcessFolder( TiXmlElement& inoParentElement, boost::shared_ptr<class CFolder> inoFolder, bool infIsRoot ) {
	using boost::shared_ptr;

	TiXmlElement* pThisElement;
	if( !infIsRoot ) {
		pThisElement = new TiXmlElement( "Folder" );
		pThisElement->SetAttribute( " name", inoFolder->GetName().c_str() );
		inoParentElement.LinkEndChild( pThisElement );
	} else
		pThisElement = &inoParentElement;
	CFolder::TFileList oFiles = inoFolder->GetFiles();
	for( CFolder::TFileList::iterator oFile = oFiles.begin(); oFile != oFiles.end(); oFile++ ) {
		TiXmlElement* pFileElement = new TiXmlElement( "File" );
		pFileElement->SetAttribute("size", oFile->size );
		TiXmlText *pText = new TiXmlText( oFile->name.c_str() );
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

	const char* pFilename = "temp.xml";
	
	m_oBuilder.SaveDocAsFile(pFilename);

	ifstream oStream( pFilename, ios::in | ios::binary );
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

};		//end namespace syncro
