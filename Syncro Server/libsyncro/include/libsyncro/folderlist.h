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

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	A struct containing info about a folder
//!
///////////////////////////////////////////////////////////////////////////////
struct FolderInfo
{
	FolderInfo() : Id( -1 ) {};

	//!
	//!	\brief	Constructor for FolderInfo
	//!
	//!	\param	id				The id of this folder
	//!	\param	name			The name of this folder
	//!	\param	path			The path to this folder
	//!	\param	uploadPrefix	The upload prefix of this folder
	//!
	FolderInfo( 
		int id, 
		const std::string& name, 
		const std::string& path,
	    const std::string& uploadPrefix = std::string()
		) :
	Id( id ),
	Name( name ),
	Path( path ),
	UploadPrefix( uploadPrefix )
	{};

	int Id;
	std::string Name;
	std::string Path;
	std::string UploadPrefix;
};

//!	Typedef for a vector of FolderInfos
typedef std::vector< FolderInfo > FolderList;

}	// namespace syncro

#endif
