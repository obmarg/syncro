#ifndef _FOLDER_MAN_H_
#define _FOLDER_MAN_H_

#include "common.h"
#include <libsyncro/folderlist.h>
#include <kode/utils.h>
#include <kode/Database.h>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <string>
#include <vector>


namespace syncro {

class CBinaryDataRequest;
class FileTransferDetails;
class UploadFinishDetails;
class DownloadFinishDetails;
class CFolder;

class CFolderMan {
public:

	CFolderMan( kode::db::Database::TPointer inpDB );
	~CFolderMan();

	const FolderList& GetFoldersInfo() const { return m_folders; };

	boost::shared_ptr<CFolder> GetFolder( int nFolderID );

	bool FileRequested( const CBinaryDataRequest& fileData, FileTransferDetails& details );

	bool IncomingFile( const CBinaryDataRequest& fileData, FileTransferDetails& details );
	
private:
	FolderList m_folders;
	kode::db::Database::TPointer m_pDB;

	kode::db::StatementPtr m_addOneShot;
	kode::db::StatementPtr m_listOneShots;
	kode::db::StatementPtr m_findOneShot;
	kode::db::StatementPtr m_delOneShot;

	typedef boost::shared_ptr< UploadFinishDetails > UploadFinishDetailsPtr;
	typedef boost::shared_ptr< DownloadFinishDetails > DownloadFinishDetailsPtr;

	const FolderInfo& FindFolder( int nFolderId );

	void FileUploadFinished( UploadFinishDetailsPtr details );
	void FileDownloadFinished( DownloadFinishDetailsPtr details );
};

class FileTransferDetails {
	friend class CFolderMan;
public:
	std::string Filename() { return m_filename; }
	VoidCallback CompletionCallback()
	{ return m_callback; }

private:
	std::string m_filename;
	VoidCallback m_callback;
};

};

#endif
