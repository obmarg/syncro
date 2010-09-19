#ifndef _SYNCRO_SERVER_H_
#define _SYNCRO_SERVER_H_

#include "cWebem.h"
#include "FolderMan.h"
#include <boost/thread.hpp>

class CSyncroServer {
public:
	CSyncroServer();
	~CSyncroServer();

	bool Run();
	char* GetFolderXML();
	char* GetFolderContentsXML();
private:
	http::server::cWebem m_oHTTP;
	std::vector< char > m_oOutputBuff;
	CFolderMan m_oFolderMan;
	boost::thread m_oBroadcastThread;

	void ProcessFolder( class TiXmlElement& inoParentElement, class std::tr1::shared_ptr<class CFolder> inoFolder );

	char *XmlToString( class TiXmlDocument& inoDoc );
};

#endif