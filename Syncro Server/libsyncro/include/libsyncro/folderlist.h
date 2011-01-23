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

#ifndef SYNCRO_FOLDERLIST_H_
#define SYNCRO_FOLDERLIST_H_

#include <string>
#include <vector>

namespace syncro
{

struct FolderInfo
{
	FolderInfo() : Id( -1 ) {};

	FolderInfo( int innID, const std::string& insName, const std::string& insPath ) :
		Id( innID ),
		Name( insName ),
		Path( insPath ),
		UploadPrefix( "" )
	{};

	FolderInfo(
	    int innID,
	    const std::string& insName,
	    const std::string& insPath,
	    const std::string& uploadPrefix
	) :
		Id( innID ),
		Name( insName ),
		Path( insPath ),
		UploadPrefix( uploadPrefix )
	{};

	int Id;
	std::string Name;
	std::string Path;
	std::string UploadPrefix;
};

typedef std::vector< FolderInfo > FolderList;

}	// namespace syncro

#endif
