#ifndef _FOLDER_MAN_H_
#define _FOLDER_MAN_H_

#include "Folder.h"
#include <libsyncro/folderlist.h>
#include <kode/utils.h>
#include <kode/Database.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>


namespace syncro {

class CBinaryDataRequest;

class CFolderMan {
public:
	CFolderMan( kode::db::Database::TPointer inpDB );
	~CFolderMan();

	const FolderList& GetFoldersInfo() const { return m_folders; };

	boost::shared_ptr<CFolder> GetFolder( int nFolderID );

	std::string GetFileName(int nFolderId,const std::string& fileName);

	std::string IncomingFile( const CBinaryDataRequest& fileData );
	
private:
	FolderList m_folders;
	kode::db::Database::TPointer m_pDB;

	const FolderInfo& FindFolder( int nFolderId );
};

};

#endif
