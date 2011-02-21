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

#include "libsyncro/scanner.h"
#include "libsyncro/comms.h"
#include <kode/net.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

namespace syncro
{
namespace client
{

class ServerDetailsImp : public Scanner::ServerDetails
{
public:
	ServerDetailsImp(
	    std::string name,
	    std::string hostname,
	    unsigned int port
	) :
		m_name( name ),
		m_hostname( hostname ),
		m_port( port )
	{
	}

	virtual const std::string& Name() const
	{
		return m_name;
	};
	virtual ConnectionDetails GetConnDetails() const
	{
		return ConnectionDetails()
		       .SetHostname( m_hostname )
		       .SetPort( m_port );
	};
private:
	std::string m_name;
	std::string m_hostname;
	unsigned int m_port;
};

Scanner::Scanner() :
	m_response( 100, 0 ),
	m_responseBuffer( 100 )
{

};

Scanner::~Scanner()
{

};

void Scanner::Scan( unsigned int timeout )
{
	using namespace boost::asio::ip;
	boost::asio::io_service service;
	udp::socket socket( service );
	//TODO: Maybe make the address selection slightly more intelligent here?
	udp::endpoint dest( address_v4::broadcast(), comms::BROADCAST_SERVER_PORT );
	udp::endpoint local( address_v4::any(), comms::BROADCAST_RESPONSE_PORT );

	boost::system::error_code error;
	socket.open( udp::v4(), error );
	socket.set_option( boost::asio::socket_base::broadcast( true ) );
	//socket.bind( local, error );

	if( error )
		throw kode::net::NetworkException( error );

	socket.send_to(
	    boost::asio::buffer( comms::BROADCAST_DISCOVERY_STRING ),
	    dest,
	    0,
	    error
	);

	if( error )
		throw kode::net::NetworkException( error );

	m_finished = false;
	boost::asio::deadline_timer t(
	    service,
	    boost::posix_time::milliseconds( timeout )
	);
	t.async_wait( boost::bind(
	                  &Scanner::Finished,
	                  this
	              ) );
	do
	{
		udp::endpoint sender;
		//TODO: This buffer size could probably be a lot better.
		socket.async_receive_from(
		    boost::asio::buffer( m_responseBuffer ),
		    m_sender,
		    0,
		    boost::bind(
		        &Scanner::HandleReceive,
		        this,
		        boost::asio::placeholders::error,
		        boost::asio::placeholders::bytes_transferred
		    )
		);
		service.run_one( error );
		if( error )
			throw new kode::net::NetworkException( error );
	}
	while( !m_finished );
}

void Scanner::Reset()
{
	m_servers.clear();
}

bool Scanner::HandleReceive(
    const boost::system::error_code& error,
    std::size_t size
)
{
	m_response = std::string(
	                 m_responseBuffer.begin(),
	                 m_responseBuffer.begin() + size
	             );

	if( error )
		throw kode::net::NetworkException( error );
	int ok =
	    m_response.substr(
	        0,
	        comms::BROADCAST_RESPONSE_PREFIX.length()
	    ).compare( comms::BROADCAST_RESPONSE_PREFIX );
	if( ok == 0 )
	{
		std::string serverName =
		    m_response.substr(
		        comms::BROADCAST_RESPONSE_PREFIX.length(),
		        m_response.npos
		    );
		boost::system::error_code error2;
		std::string address = m_sender.address().to_string( error2 );
		if( error2 )
			throw kode::net::NetworkException( error );

		ServerDetailsPtr newVal(
		    new ServerDetailsImp(
		        serverName, address, comms::SERVER_PORT
		    )
		);
		m_servers.insert( newVal );
	}
	return true;
}

}	// namespace client
}	// namespace syncro
