#ifndef UPLOADTHREAD_H_
#define UPLOADTHREAD_H_

#include <libsyncro/connection.h>

class UploadThread
{
public:
	static void Create(
	    const syncro::client::ConnectionDetails& connDetails,
	    const syncro::client::UploadFileDetails& uploadDetails
	);

private:
	UploadThread(
	    const syncro::client::ConnectionDetails& connDetails,
	    const syncro::client::UploadFileDetails& uploadDetails
	);

	static void ThreadFunc(
	    const syncro::client::ConnectionDetails& connDetails,
	    const syncro::client::UploadFileDetails& uploadDetails
	);

	void Run();

private:
	syncro::client::Connection m_conn;
	syncro::client::UploadFileDetails m_uploadDetails;
};

#endif