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

#include "BroadcastThread.h"
#include "BroadcastListener.h"
#include <boost/asio/ip/host_name.hpp>
#include "SyncroDB.h"

namespace syncro
{

BroadcastThread::BroadcastThread(
	bool run
	) :
m_run( run )
{
	//TODO: get the hostname from the database or something
	m_sServerName = boost::asio::ip::host_name();
}

BroadcastThread::~BroadcastThread()
{

}

void
BroadcastThread::operator()()
{
	if( !m_run )
		return;
	boost::asio::io_service oIOService;
	BroadcastListener oListener( oIOService, m_sServerName );
	oIOService.run();
}

};
