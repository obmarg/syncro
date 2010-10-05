#ifndef _FOLDER_MAN_H_
#define _FOLDER_MAN_H_

#include <string>
#include <vector>
#include "utils.h"
#include <boost\shared_ptr.hpp>
#include "Folder.h"

namespace syncro {

class CFolderMan {
public:
	struct sFolderInfo {
		sFolderInfo( int innID, const std::string& insName ) : nFolderID( innID ), sFolderName( insName ) {};
		int nFolderID;
		std::string sFolderName;
	};

	typedef std::vector<sFolderInfo> TFolderInfoList;

	CFolderMan( const std::string& insRoot );
	~CFolderMan();

	
	const TFolderInfoList& GetFoldersInfo() { return m_aFolderInfo; };

	boost::shared_ptr<CFolder> GetFolder( int nIndex );

	std::string GetFileName(int nFolderIndex,const std::string& fileName);
	
private:
	std::string m_sRoot;
	TFolderInfoList m_aFolderInfo;
};

};

#endif