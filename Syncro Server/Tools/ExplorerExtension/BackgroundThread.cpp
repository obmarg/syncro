#include "stdafx.h"
#include "BackgroundThread.h"
#include <libsyncro/connection.h>
#include <libsyncro/scanner.h>

BackgroundThread& BackgroundThread::GetInstance()
{
	static BackgroundThread instance;
	return instance;
}

BackgroundThread::BackgroundThread()
{
	m_thread = boost::thread( boost::bind(
	                              &BackgroundThread::RunThread,
	                              this
	                          ) );
}

BackgroundThread::~BackgroundThread()
{
	//Wait for the background thread to finish
	//TODO: Probably a better way to do this.  some sort
	//		of termination call
	m_thread.join();
}

//
// Attempts to lock the mutex, then calls the callback.
// Returns instantly otherwise
//
bool
BackgroundThread::GetData( ServerListCallback callback )
{
	try
	{
		boost::mutex::scoped_try_lock lock( m_dataMutex );

		callback( m_servers );

		return true;
	}
	catch( const boost::lock_error& ex )
	{
		ex.what();
	}
	return false;
}

void
BackgroundThread::RunThread()
{
	using syncro::client::Scanner;
	using syncro::client::Connection;
	using syncro::client::ConnectionDetails;

	Scanner scanner;
	try
	{
		scanner.Scan( 500 );
	}
	catch( const std::exception& ex )
	{
		ex.what();
		return;
	}
	BOOST_FOREACH( Scanner::ServerDetailsPtr details, scanner.Servers() )
	{
		try
		{
			boost::mutex::scoped_lock( m_dataMutex );
			Connection conn(( *details ) );
			m_servers.push_back( ServerInfo( details->Name(), *details ) );
			conn.GetFolderList( m_servers.back().folders );
		}
		catch( const std::exception& ex )
		{
			ex.what();
		}
	}
}