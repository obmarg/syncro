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

#include "stdafx.h"
#include "UploadThread.h"
#include <boost/thread.hpp>

void UploadThread::Create(
    const syncro::client::ConnectionDetails& connDetails,
    const syncro::client::FileTransferDetails& uploadDetails
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
    const syncro::client::FileTransferDetails& uploadDetails
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
    const syncro::client::FileTransferDetails& uploadDetails
) :
	m_conn( connDetails ),
	m_uploadDetails( uploadDetails )
{

}

void UploadThread::Run()
{
	m_conn.UploadFile( m_uploadDetails );
}

