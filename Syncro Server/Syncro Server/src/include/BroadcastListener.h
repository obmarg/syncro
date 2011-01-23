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

#ifndef _BROADCAST_LISTENER_H_
#define _BROADCAST_LISTENER_H_

#include <boost/asio.hpp>

namespace syncro
{

class CBroadcastListener
{
public:
	CBroadcastListener( boost::asio::io_service& inoIOService, const std::string& insServerName );
	~CBroadcastListener();

private:
	boost::asio::ip::udp::socket m_oSocket;
	boost::asio::ip::udp::endpoint m_oEndpoint;
	boost::array<char, 20> m_aRecvBuff;

	const std::string m_sServerName;

	void HandleReceive( const boost::system::error_code& error, std::size_t );
	void StartRecieve();
};

}; //namespace syncro

#endif