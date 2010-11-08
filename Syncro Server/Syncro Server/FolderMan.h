#ifndef _FOLDER_MAN_H_
#define _FOLDER_MAN_H_

#include <kode/utils.h>
#include <kode/Database.h>
#include "Folder.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>


namespace syncro {

class CBinaryDataRequest;

class CFolderMan {
public:
	struct sFolderInfo {
		sFolderInfo( int innID, const std::string& insName ) : nFolderID( innID ), sFolderName( insName ) {};
		int nFolderID;
		std::string sFolderName;
	};

	typedef std::vector<sFolderInfo> TFolderInfoList;

	CFolderMan( kode::db::Database::TPointer inpDB );
	~CFolderMan();

	
	const TFolderInfoList& GetFoldersInfo() { return m_aFolderInfo; };

	boost::shared_ptr<CFolder> GetFolder( int nFolderID );

	std::string GetFileName(int nFolderId,const std::string& fileName);

	std::string IncomingFile( const CBinaryDataRequest& fileData );
	
private:
	TFolderInfoList m_aFolderInfo;
	kode::db::Database::TPointer m_pDB;

	const sFolderInfo& FindFolder( int nFolderId );
};

};

#endif
