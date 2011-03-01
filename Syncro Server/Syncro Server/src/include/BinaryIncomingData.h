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

#ifndef _BINARY_INCOMING_DATA_H_
#define _BINARY_INCOMING_DATA_H_

#include "common.h"
#include <kode/stopwatch.h>
#include <fstream>

namespace syncro
{

class CBinaryIncomingData
{
public:
	CBinaryIncomingData(
	    const std::string sFilename,
	    VoidCallback completedCallback
	);
	~CBinaryIncomingData();

	void HandlePacket( InputStreamList& inaInputStreams );

	void ShouldResume(int64_t resumePoint);

	std::string Filename() const
	{ return m_filename; };

private:
	void StartFile(int64_t startOffset=0);

private:
	std::ofstream m_oFile;
	std::string m_filename;
	bool m_started;
	VoidCallback m_completedCallback;
	kode::time::Stopwatch m_stopwatch;
	unsigned int m_dataTransferred;
};

}

#endif