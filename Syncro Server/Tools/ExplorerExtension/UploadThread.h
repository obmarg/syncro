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

#ifndef UPLOADTHREAD_H_
#define UPLOADTHREAD_H_

#include <libsyncro/connection.h>

class UploadThread
{
public:
	static void Create(
	    const syncro::client::ConnectionDetails& connDetails,
	    const syncro::client::FileTransferDetails& uploadDetails
	);

private:
	UploadThread(
	    const syncro::client::ConnectionDetails& connDetails,
	    const syncro::client::FileTransferDetails& uploadDetails
	);

	static void ThreadFunc(
	    const syncro::client::ConnectionDetails& connDetails,
	    const syncro::client::FileTransferDetails& uploadDetails
	);

	void Run();

private:
	syncro::client::Connection m_conn;
	syncro::client::FileTransferDetails m_uploadDetails;
};

#endif