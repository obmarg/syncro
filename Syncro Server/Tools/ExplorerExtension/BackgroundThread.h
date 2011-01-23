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

#ifndef BACKGROUND_THREAD_H_
#define BACKGROUND_THREAD_H_

#include <libsyncro/folderlist.h>
#include <libsyncro/connection.h>
#include <boost/thread.hpp>
#include <vector>

class BackgroundThread
{
public:
	struct ServerInfo
	{
	public:
		ServerInfo(
		    std::string inname,
		    const syncro::client::ConnectionDetails& conn
		) :
			Name( inname ),
			connDetails( conn )
		{ };

		std::string Name;
		syncro::client::ConnectionDetails connDetails;
		syncro::FolderList folders;
	};
	typedef std::vector<ServerInfo> ServerList;

	typedef boost::function <
	void ( const ServerList& )
	> ServerListCallback;

public:
	BackgroundThread();
	~BackgroundThread();

	//
	// Attempts to lock the mutex, then calls the callback.
	// Returns instantly otherwise
	//
	bool GetData( ServerListCallback callback );

	static BackgroundThread& GetInstance();

private:
	void RunThread();

	boost::mutex m_dataMutex;
	boost::thread m_thread;

	ServerList m_servers;
};

#endif