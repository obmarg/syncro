#ifndef _XML_BUILDER_H_
#define _XML_BUILDER_H_

#include "common.h"
#include "tinyxml.h"

class TiXmlDocument;

class CXMLBuilder {
public:
	CXMLBuilder();
	~CXMLBuilder();

	void SaveDocAsFile(std::string inoFile);

	bool GetFolderXML();
	bool GetFolderContentsXML();
private:
	TiXmlDocument m_oXML;
};

class CXMLOutput {
public:
	CXMLOutput(CXMLBuilder& inoBuilder) : m_oBuilder(inoBuilder) {

	}
	void operator()(TCharBuffer::TBuff& inoBuffer);
private:
	CXMLBuilder& m_oBuilder;
};


#endif