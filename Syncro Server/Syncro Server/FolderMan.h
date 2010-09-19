#ifndef _FOLDER_MAN_H_
#define _FOLDER_MAN_H_

#include <string>
#include <vector>
#include "utils.h"
#include <memory>
#include "Folder.h"

class CFolderMan {
public:

	CFolderMan( const std::string& insRoot );
	~CFolderMan();

	const std::vector<std::string>& GetFolders() { return m_oFolders; };

	std::tr1::shared_ptr<CFolder> GetFolder( int nIndex );
	
private:
	std::string m_sRoot;
	std::vector<std::string> m_oFolders;
};

#endif