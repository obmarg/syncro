#ifndef _XML_BUILDER_H_
#define _XML_BUILDER_H_

#include "common.h"
#include "tinyxml.h"
#include <boost/shared_ptr.hpp>

class TiXmlDocument;

namespace syncro {

class CXMLBuilder {
public:
	CXMLBuilder();
	~CXMLBuilder();

	void SaveDocAsFile(std::string inoFile);

	bool GetFolderXML();
	bool GetFolderContentsXML(int innFolderID);
private:
	TiXmlDocument m_oXML;

	void ProcessFolder( TiXmlElement& inoParentElement, boost::shared_ptr<class CFolder> inoFolder, bool infIsRoot = false );

};

class CXMLOutput {
public:
	CXMLOutput(CXMLBuilder& inoBuilder) : m_oBuilder(inoBuilder) {

	}
	CXMLOutput(const CXMLOutput& inoOther) : m_oBuilder( inoOther.m_oBuilder ) {};

	void operator()(TCharBuffer::TBuff& inoBuffer);
private:
	CXMLBuilder& m_oBuilder;
};

};		//namespace syncro


#endif