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

#include "BroadcastListener.h"
#include <libsyncro/comms.h>
#include <boost/bind.hpp>

namespace syncro
{

using boost::asio::ip::udp;
using std::string;

const std::string RESPOND_MESSAGE = "SyncroHELLO";

BroadcastListener::BroadcastListener( boost::asio::io_service& inoIOService, const std::string& insServerName )
	: m_oSocket( inoIOService, udp::endpoint( udp::v4(), comms::BROADCAST_SERVER_PORT ) ),
	  m_sServerName( insServerName )
{
	StartRecieve();
}

BroadcastListener::~BroadcastListener()
{

}

void
BroadcastListener::HandleReceive( const boost::system::error_code& error, std::size_t innSize )
{
	bool fOK = false;
	if( innSize >= RESPOND_MESSAGE.length() )
	{
		fOK = true;
		for( unsigned int nChar = 0; nChar < RESPOND_MESSAGE.length(); nChar++ )
		{
			if( RESPOND_MESSAGE[nChar] != m_aRecvBuff[nChar] )
				fOK = false;
		}
	}
	if( fOK )
	{
		string oResponse = comms::BROADCAST_RESPONSE_PREFIX + m_sServerName;
		m_oSocket.send_to( boost::asio::buffer( oResponse ), m_oEndpoint );
	}
	StartRecieve();
}

void
BroadcastListener::StartRecieve()
{
	m_oSocket.async_receive_from(
	    boost::asio::buffer( m_aRecvBuff ), m_oEndpoint,
	    boost::bind( &BroadcastListener::HandleReceive, this,
	                 boost::asio::placeholders::error,
	                 boost::asio::placeholders::bytes_transferred ) );
}

};	//namespace syncro
