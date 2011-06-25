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

#ifndef _BINARY_DATA_REQUEST_H_
#define _BINARY_DATA_REQUEST_H_

#include "common.h"
#include <stdint.h>


namespace syncro
{

class BinaryDataRequest
{
public:
	BinaryDataRequest( int folderId, const std::string& filename );
	BinaryDataRequest( const InputStreamList& inaInputStreams );
	~BinaryDataRequest() {};

	const std::string& GetFilename() const
	{ return m_sFilename; }

	const int GetFolderId() const
	{ return m_nFolderId; }

	const int GetBufferSize() const
	{ return m_nBufferSize; }

	const int64_t GetFileSize() const
	{ return m_nFileSize; }

	const bool IsOneShot() const
	{ return m_oneShot; };

	const int64_t GetStartOffset() const
	{ return m_startOffset; }

	const int64_t GetModifiedTime() const
	{ return m_modifiedTime; }

private:
	std::string m_sFilename;
	int m_nFolderId;
	int m_nBufferSize;
	int64_t m_nFileSize;
	bool m_oneShot;
	int64_t m_startOffset;
	int64_t m_modifiedTime;
};

};	//namespace syncro

#endif
