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

#include "FolderMan.h"
#include "Folder.h"
#include "BinaryDataRequest.h"
#include "TransferFinishDetails.h"
#include "Config.h"
#include "Logging.h"
#include <libsyncro/stringutils.h>
#include <kode/db/statement.h>
#include <kode/utils.h>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <string>
#include <iostream>

namespace syncro
{

using namespace std;
using namespace boost::filesystem;
using namespace kode::db;

FolderMan::FolderMan( DatabasePtr db ) : m_db( db )
{
	StatementPtr statement = 
		m_db->prepare( "SELECT ID,Name,Path,UploadPrefix FROM Folders");

	while( statement->GetNextRow() )
	{
		path oPath( statement->GetColumn< std::string >( "Path" ) );
		oPath = boost::filesystem::absolute( oPath );
		if( !is_directory( oPath ) )
			throw std::runtime_error( "Invalid path read from DB in CFolderMan constructor" );
		//TODO: Do something with the name as well
		m_folders.push_back(
		    FolderInfo(
		        statement->GetColumn< int >( "ID" ),
		        statement->GetColumn< std::string >( "Name" ),
		        oPath.string(),
		        statement->GetColumn< std::string >( "UploadPrefix" )
		    )
		);
		if(( *m_folders.back().Path.rbegin() ) != '/'
		        && ( *m_folders.back().Path.rbegin() ) != '\\' )
		{
			m_folders.back().Path += kode::utils::PATH_SEPERATOR_STR;
		}
	}
}

FolderMan::~FolderMan()
{

}

const FolderInfo& FolderMan::FindFolder( int nFolderId )
{
	BOOST_FOREACH( const FolderInfo & oInfo, m_folders )
	{
		if( oInfo.Id == nFolderId )
		{
			return oInfo;
		}
	}
	throw std::range_error( "Invalid folder ID passed to CFolderMan" );
}


boost::shared_ptr<Folder>
FolderMan::GetFolder( int nFolderID )
{
	const FolderInfo& info = FindFolder( nFolderID );
	boost::shared_ptr<Folder> rv(
	    new Folder( info.Path, info.Name )
	);
	//TODO: handle exception here or elsewhere?

	if( !m_listOneShots )
	{
		m_listOneShots = m_db->prepare(
		                     "SELECT FileName,LocalPath FROM files "
		                     "WHERE OneShot=1 AND FolderID=?"
		                 );
	}
	kode::db::AutoReset reset( m_listOneShots );
	m_listOneShots->Bind( 1, nFolderID );
	while( m_listOneShots->GetNextRow() )
	{
		std::string localPath = m_listOneShots->GetColumn< std::string >( 1 );
		std::string name =  m_listOneShots->GetColumn< std::string >( 0 );
		path filePath( localPath );
		if( exists( filePath ) )
		{
			uint32_t size =
			    boost::numeric_cast<uint32_t>( file_size( filePath ) );
			rv->AddFile( name, size );
		}
	}
	return rv;
}

bool
FolderMan::FileRequested(
    const BinaryDataRequest& requestData,
    FileTransferDetails& details
)
{
	std::string folderPath =
	    FindFolder( requestData.GetFolderId() ).Path;

    boost::filesystem::path fileName = folderPath;

	fileName /= requestData.GetFilename();
	bool oneShot = false;
	int oneShotId = -1;
	bool deleteOnFinish = false;
	if( !exists( fileName ) )
	{
		if( !m_findOneShot )
		{
			m_findOneShot = m_db->prepare(
			                    "SELECT ID,LocalPath,OneShot,DeleteOnFinish"
								" FROM files "
			                    "WHERE FolderID=? AND FileName=?"
			                );
		}
		kode::db::AutoReset reset( m_findOneShot );
		m_findOneShot->Bind( 1, requestData.GetFolderId() );
		m_findOneShot->Bind( 2, requestData.GetFilename() );
		if( m_findOneShot->GetNextRow() )
		{
			//We've found a one shot file, sweet...
			oneShotId = m_findOneShot->GetColumn< int >( 0 );
			fileName = m_findOneShot->GetColumn< std::string >( 1 );
			oneShot = m_findOneShot->GetColumn< int >( 2 ) != 0;
			deleteOnFinish = m_findOneShot->GetColumn< int >( 3 ) != 0;
		}
		else
		{
			return false;
		}
	}
	std::string absoluteFileName = 
		boost::filesystem::absolute( fileName ).string();
	//
	// Ensure that the absolute file name starts with the folder path
	// for security purposes
	//
	if( 
            !oneShot && 
            absoluteFileName.compare( 0, folderPath.size(), folderPath ) != 0 
            )
	{
        throw std::runtime_error( 
            "Client requested to download file outside of container folder"
            );
	}
	auto fileNameString = fileName.string();
	details.m_filename = fileNameString;
	details.m_modifiedTime = 
		boost::numeric_cast< int64_t >( 
			boost::filesystem::last_write_time( fileName )
			);
	//TODO: Add support for folder path in here. for now doesn't matter
	//		can possibly remove the parameter if it turns out just the
	//		name is good enough
	DownloadFinishDetailsPtr finishDetails(
	    new DownloadFinishDetails(
	        requestData.GetFilename(),
	        oneShot,
	        requestData.GetFolderId(),
	        "",
	        fileNameString,
			deleteOnFinish,
	        oneShotId
	    )
	);
	details.m_callback = boost::bind(
	                         &FolderMan::FileDownloadFinished,
	                         this,
	                         finishDetails
	                     );
	details.m_currentSize = requestData.GetStartOffset();
	return true;
}

bool
FolderMan::IncomingFile(
    const BinaryDataRequest& fileData,
    FileTransferDetails& details
)
{
	const FolderInfo& folderInfo = FindFolder( fileData.GetFolderId() );
    boost::filesystem::path rootPath = folderInfo.Path;
	if( fileData.IsOneShot() )
	{
		//If one shot, then we need to upload to a temporary folder
		rootPath = Config::GetInstance().TempFilesPath();
	}
	else if( !folderInfo.UploadPrefix.empty() )
	{
		std::string prefix = folderInfo.UploadPrefix;
		syncro::utils::ReplaceStringVars( prefix );
		rootPath /= prefix;
	}
	auto destFile = rootPath / fileData.GetFilename();
	auto destDir = destFile;
	destDir.remove_filename();
	if( !boost::filesystem::exists( destDir ) )
	{
		boost::filesystem::create_directories( destDir );
	}
	details.m_filename = 
		boost::filesystem::absolute( destFile ).string();
	//TODO: Add support for folder path in here. for now doesn't matter
	//		can possibly remove the parameter if it turns out just the
	//		name is good enough
	UploadFinishDetailsPtr finishDetails(
	    new UploadFinishDetails(
	        fileData.GetFilename(),
	        fileData.IsOneShot(),
	        fileData.GetFolderId(),
	        "",
	        details.m_filename,
			fileData.IsOneShot(),
			fileData.GetModifiedTime()
	    )
	);
	details.m_callback = boost::bind(
	                         &FolderMan::FileUploadFinished,
	                         this,
	                         finishDetails
	                     );
	int64_t nFileSize = fileData.GetFileSize();
	if( exists( destFile ) && nFileSize != -1 )
	{
		if( file_size( destFile ) != nFileSize )
		{
			// File size doesn't match, definetly download again.
			details.m_currentSize = file_size( destFile );
			return true;
		}
	}
	//
	// Check the database to see if this has been uploaded already
	//
	if( !m_checkUploadHistory )
	{
		m_checkUploadHistory = m_db->prepare(
		                           "SELECT ID,ActualFilename FROM UploadHistory "
		                           "WHERE FolderID=? AND Filename=?"
		                       );
	}
	bool accept = true;
	kode::db::AutoReset reset( m_checkUploadHistory );
	m_checkUploadHistory->Bind( 1, fileData.GetFolderId() );
	m_checkUploadHistory->Bind( 2, fileData.GetFilename() );
	while( m_checkUploadHistory->GetNextRow() )
	{
		boost::filesystem::path otherFile(
		    m_checkUploadHistory->GetColumn<std::string>( 1 )
		);
		if( exists( otherFile ) )
		{
			if( file_size( otherFile ) == nFileSize )
			{
				accept = false;
			}
		}
	}
	//TODO: SHould i just throw an exception here?
	return accept;
}

void FolderMan::FileUploadFinished( UploadFinishDetailsPtr details )
{
	if( details->oneShot )
	{
		//If we're a one shot file, we need to add a database entry
		if( !m_addOneShot )
		{
			m_addOneShot = m_db->prepare(
			                   "INSERT INTO Files "
			                   "(Filename,FolderPath,LocalPath,FolderId,"
							   "OneShot,DeleteOnFinish) "
			                   "VALUES (?, ?, ?, ?, 1, ?);"
								);
		}
		kode::db::AutoReset reset( m_addOneShot );
		m_addOneShot->Bind( 1, details->fileName );
		m_addOneShot->Bind( 2, details->folderPath );
		m_addOneShot->Bind( 3, details->localPath );
		m_addOneShot->Bind( 4, details->folderId );
		m_addOneShot->Bind( 5, details->deleteOnFinish );
		m_addOneShot->GetNextRow();
		//TODO: Maybe after commiting that row, query it, and get the ID
		//		then move the file to a prefix of that id (or just id for
		//		filename)  to save overwriting things accidentally.
	}
	else
	{
		if( !m_addToUploadHistory )
		{
			m_addToUploadHistory = m_db->prepare(
			                           "INSERT INTO UploadHistory "
			                           "(Filename, FolderId, ActualFilename) "
			                           "VALUES( ?, ?, ?);"
			                       );
		}
		kode::db::AutoReset reset( m_addToUploadHistory );
		m_addToUploadHistory->Bind( 1, details->fileName );
		m_addToUploadHistory->Bind( 2, details->folderId );
		m_addToUploadHistory->Bind( 3, details->localPath );
		m_addToUploadHistory->GetNextRow();
	}

	if( details->modifiedTime != 0 )
	{
		try
		{
			boost::filesystem::last_write_time( 
				details->localPath,
				boost::numeric_cast< time_t >( details->modifiedTime )
				);
		}
		catch( const std::exception& ex )
		{
            log::warning << "Failed to set last write time in "
				<< "CFolderMan::FileUploadFinished." << "\n"
				<< "What: " << ex.what() << "\n";
		}
	}
	else
	{
        log::warning << "Warning: No modification time in FileUploadFinished"
			<< "\n";
	}

	//TODO: At some point, would be good to clear out the old uploaded files database,
	//		based on clients no longer attempting to upload the file.
	//		Could keep a list of files requested upload, then on destruction
	//		delete anything that's not in the list...
}

void FolderMan::FileDownloadFinished( DownloadFinishDetailsPtr details )
{
	if( details->oneShot )
	{
		if( !m_delOneShot )
		{
			m_delOneShot = m_db->prepare(
			                   "DELETE FROM Files WHERE OneShot=1 AND ID=?"
			               );
		}
		kode::db::AutoReset reset( m_delOneShot );
		m_delOneShot->Bind( 1, details->fileId );
		m_delOneShot->GetNextRow();
	}
	if( details->deleteOnFinish )
	{
		boost::filesystem::remove( details->localPath );
	}
}

};		//end namespace syncro
