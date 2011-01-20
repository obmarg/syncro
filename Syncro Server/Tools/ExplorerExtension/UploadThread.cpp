#include "stdafx.h"
#include "UploadThread.h"
#include <boost/thread.hpp>

void UploadThread::Create(
    const syncro::client::ConnectionDetails& connDetails,
    const syncro::client::UploadFileDetails& uploadDetails
)
{
	boost::thread thread(
	    boost::bind(
	        &UploadThread::ThreadFunc,
	        connDetails,
	        uploadDetails
	    )
	);
}

void UploadThread::ThreadFunc(
    const syncro::client::ConnectionDetails& connDetails,
    const syncro::client::UploadFileDetails& uploadDetails
)
{
	try
	{
		UploadThread object( connDetails, uploadDetails );
		object.Run();
	}
	catch( const std::exception& )
	{
		//TODO: Handle exceptions
	}
}

UploadThread::UploadThread(
    const syncro::client::ConnectionDetails& connDetails,
    const syncro::client::UploadFileDetails& uploadDetails
) :
	m_conn( connDetails ),
	m_uploadDetails( uploadDetails )
{

}

void UploadThread::Run()
{
	m_conn.UploadFile( m_uploadDetails );
}

