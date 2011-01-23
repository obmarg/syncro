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

#ifndef _FOLDER_H_
#define _FOLDER_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace syncro
{

class CFolder
{
public:
	struct sFileData
	{
		sFileData(
		    const std::string& insName,
		    const unsigned int innSize
		) :
			name( insName ),
			size( innSize )
		{ };

		std::string name;
		unsigned int size;
	};

	typedef std::vector< sFileData > TFileList;

	CFolder( const std::string& insPath, const std::string& name );
	~CFolder();

	std::string GetName()
	{
		return m_sFolderName;
	}
	std::string GetPath()
	{
		return m_sPath;
	}
	const TFileList& GetFiles()
	{
		return m_oFiles;
	};
	std::vector< boost::shared_ptr<CFolder> >& GetChildren()
	{
		return m_oChildren;
	}

	void AddFile( const std::string& name, unsigned int size );

private:
	std::string m_sPath;
	std::string m_sFolderName;
	std::vector< boost::shared_ptr<CFolder> > m_oChildren;
	TFileList m_oFiles;
};

}


#endif
