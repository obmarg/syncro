/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

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


namespace syncro
{

class CBinaryDataRequest;
class FileTransferDetails;
class UploadFinishDetails;
class DownloadFinishDetails;
class CFolder;

class CFolderMan
{
public:

	CFolderMan( kode::db::Database::TPointer inpDB );
	~CFolderMan();

	const FolderList& GetFoldersInfo() const
	{
		return m_folders;
	};

	boost::shared_ptr<CFolder> GetFolder( int nFolderID );

	bool FileRequested( 
		const CBinaryDataRequest& fileData, 
		FileTransferDetails& details 
		);

	bool IncomingFile( const CBinaryDataRequest& fileData, FileTransferDetails& details );

private:
	FolderList m_folders;
	kode::db::Database::TPointer m_pDB;

	kode::db::StatementPtr m_addOneShot;
	kode::db::StatementPtr m_listOneShots;
	kode::db::StatementPtr m_findOneShot;
	kode::db::StatementPtr m_delOneShot;

	kode::db::StatementPtr m_addToUploadHistory;
	kode::db::StatementPtr m_checkUploadHistory;
	kode::db::StatementPtr m_clearUploadHistory;

	typedef boost::shared_ptr< UploadFinishDetails > UploadFinishDetailsPtr;
	typedef boost::shared_ptr< DownloadFinishDetails > DownloadFinishDetailsPtr;

	const FolderInfo& FindFolder( int nFolderId );

	void FileUploadFinished( UploadFinishDetailsPtr details );
	void FileDownloadFinished( DownloadFinishDetailsPtr details );
};

class FileTransferDetails
{
	friend class CFolderMan;
public:
	FileTransferDetails() :
	m_currentSize(0)
	{ }

	std::string Filename()
	{ return m_filename; }

	VoidCallback CompletionCallback()
	{ return m_callback; }

	int64_t CurrentSize()
	{ return m_currentSize; }
private:
	std::string m_filename;
	VoidCallback m_callback;
	int64_t m_currentSize;
};

};

#endif
