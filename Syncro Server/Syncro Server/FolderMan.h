#ifndef _FOLDER_MAN_H_
#define _FOLDER_MAN_H_

#include <string>
#include <vector>
#include "utils.h"
#include <boost\shared_ptr.hpp>
#include "Folder.h"
#include "Database.h"

namespace syncro {

class CFolderMan {
public:
	struct sFolderInfo {
		sFolderInfo( int innID, const std::string& insName ) : nFolderID( innID ), sFolderName( insName ) {};
		int nFolderID;
		std::string sFolderName;
	};

	typedef std::vector<sFolderInfo> TFolderInfoList;

	CFolderMan( Database::TPointer inpDB );
	~CFolderMan();

	
	const TFolderInfoList& GetFoldersInfo() { return m_aFolderInfo; };

	boost::shared_ptr<CFolder> GetFolder( int nFolderID );

	std::string GetFileName(int nFolderId,const std::string& fileName);
	
private:
	TFolderInfoList m_aFolderInfo;
	Database::TPointer m_pDB;

	const sFolderInfo& FindFolder( int nFolderId );
};

};

#endif