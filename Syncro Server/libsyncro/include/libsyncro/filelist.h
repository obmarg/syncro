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

#ifndef SYNCRO_FILE_FILELIST_H_
#define SYNCRO_FILE_FILELIST_H_

#include <string>
#include <stdint.h>

namespace syncro {

struct FileInfo {
	FileInfo( const std::string& inname, int64_t insize ) :
	name( inname ),
	size( insize )
	{ };

	std::string name;
	int64_t		size;
};

typedef std::vector< FileInfo > FileList;

}	// namespace syncro

#endif