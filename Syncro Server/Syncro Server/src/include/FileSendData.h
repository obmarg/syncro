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

#ifndef _FILE_SEND_DATA_H_
#define _FILE_SEND_DATA_H_

#include "common.h"
#include <kode/utils.h>
#include <kode/stopwatch.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <fstream>

namespace syncro
{

class FileSendData : boost::noncopyable
{
public:
	FileSendData(
	    const std::string& fileName,
	    int requestedBufferSize = 0,
	    const VoidCallback& completionCallback = VoidCallback(),
	    int64_t fileStartOffset = 0,
		int64_t fileModifiedTime = 0
	);

	~FileSendData();

	void FillBuffer( google::protobuf::io::ZeroCopyOutputStream& stream );
	unsigned int GetChunkSize();

	std::istream::pos_type GetFilePosition();
	std::istream::pos_type GetFileSize()
	{
		return m_fileSize;
	};
	int64_t GetFileModifiedTime()
	{
		return m_fileModifiedTime;
	}
	bool IsFileFinished();
	bool IsStartFile();
	bool IsFileFinishedAfterChunk( unsigned int nextChunkSize );

private:

	void OpenFile( int64_t fileStartOffset );

	void CallCompletionCallback();

	std::ifstream m_file;
	const std::string m_fileName;

	int64_t	m_fileSize;
	int64_t m_fileModifiedTime;

	unsigned int m_nRequestedBufferSize;

	kode::time::Stopwatch m_stopwatch;
	VoidCallback m_completionCallback;

	bool m_finishedAfterChunk;
};

} //end namespace syncro

#endif