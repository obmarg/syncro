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
		Name(inname),
		connDetails( conn )
		{ };

		std::string Name;
		syncro::client::ConnectionDetails connDetails;
		syncro::FolderList folders;
	};
	typedef std::vector<ServerInfo> ServerList;

	typedef boost::function<
		void (const ServerList&) 
		> ServerListCallback;

public:
	BackgroundThread();
	~BackgroundThread();

	//
	// Attempts to lock the mutex, then calls the callback.
	// Returns instantly otherwise
	//
	bool GetData(ServerListCallback callback);

	static BackgroundThread& GetInstance();

private:
	void RunThread();

	boost::mutex m_dataMutex;
	boost::thread m_thread;

	ServerList m_servers;
};

#endif