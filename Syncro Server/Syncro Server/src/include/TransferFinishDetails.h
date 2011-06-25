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

#ifndef SYNCRO_TRANSFER_FINISH_DETAILS_H_
#define SYNCRO_TRANSFER_FINISH_DETAILS_H_

namespace syncro {

struct BaseFinishDetails
{
	BaseFinishDetails(
		const std::string& inFilename,
		const bool inOneShot,
		const int inFolderId,
		const std::string& inFolderPath,
		const std::string& inLocalPath,
		const bool inDeleteOnFinish
		) :
	fileName( inFilename ),
	oneShot( inOneShot ),
	folderId( inFolderId ),
	folderPath( inFolderPath ),
	localPath( inLocalPath ),
	deleteOnFinish( inDeleteOnFinish )
	{

	}
	std::string fileName;
	bool oneShot;
	unsigned int folderId;
	std::string folderPath;
	std::string localPath;
	bool deleteOnFinish;
};

struct UploadFinishDetails : BaseFinishDetails
{
public:
	UploadFinishDetails(
		const std::string& inFilename,
		const bool inOneShot,
		const int inFolderId,
		const std::string& inFolderPath,
		const std::string& inLocalPath,
		const bool inDeleteOnFinish,
		const int64_t inModifiedTime
		) :
	BaseFinishDetails(
		inFilename,
		inOneShot,
		inFolderId,
		inFolderPath,
		inLocalPath,
		inDeleteOnFinish
		),
	modifiedTime( inModifiedTime )
	{ }

	int64_t modifiedTime;
};

// For now, DownloadFinishDetails is very similar to upload finish
//TODO: At some point, split into a BaseFinishDetails & seperate inherited
//		Download & Upload FinishDetails
struct DownloadFinishDetails : public BaseFinishDetails
{
public:
	DownloadFinishDetails(
		const std::string& inFilename,
		const bool inOneShot,
		const int inFolderId,
		const std::string& inFolderPath,
		const std::string& inLocalPath,
		bool inDeleteOnFinish,
		const int inFileId = -1
		) :
	BaseFinishDetails(
		inFilename,
		inOneShot,
		inFolderId,
		inFolderPath,
		inLocalPath,
		inDeleteOnFinish
		),
	fileId( inFileId )
	{ }

	int fileId;
};

}		// namespace syncro 

#endif	// SYNCRO_TRANSFER_FINISH_DETAILS_H_