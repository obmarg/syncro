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

#ifndef SYNCRO_FOLDER_CONTENTS_HANDLERS_H_
#define SYNCRO_FOLDER_CONTENTS_HANDLERS_H_

#include "common.h"
#include "BasePBResponse.h"
#include <boost/shared_ptr.hpp>

namespace syncro
{

class CFolderMan;
class CFolder;

namespace pb {
class FolderContents;
}

namespace pbHandlers
{

class FolderContentsRequest
{
public:
	FolderContentsRequest( 
		InputStreamList& inaInputStreams,
		CFolderMan& folderMan
		);

	CBasePBResponse::TPointer GetResponse();

private:
	typedef boost::shared_ptr<CFolder> CFolderPtr;

	void ProcessFolder( pb::FolderContents* pb, const CFolderPtr folder );

private:
	CFolderPtr m_rootFolder;
};

}	// namespace pbHandlers
}	// namespace syncro

#endif