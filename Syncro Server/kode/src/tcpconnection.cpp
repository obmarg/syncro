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

#include "kode/tcpconnection.h"
#include "kode/net.h"
#include <boost/lexical_cast.hpp>

namespace kode
{
namespace net
{

TCPConnection::TCPConnection( std::string hostname, int nPort ) :
	m_service(),
	m_socket( m_service )
{
	using namespace boost::asio;

	ip::tcp::resolver resolver( m_service );
	ip::tcp::resolver::query query(
	    hostname,
	    boost::lexical_cast< std::string >( nPort ),
	    ip::resolver_query_base::numeric_service );

	ip::tcp::resolver::iterator iter = resolver.resolve( query );
	ip::tcp::resolver::iterator endIter;

	boost::system::error_code error = boost::asio::error::host_not_found;
	while( error && iter != endIter )
	{
		m_socket.close();
		m_socket.connect( *iter, error );
		++iter;
	}
	if( error )
	{
		throw NetworkException( error );
	}
}

TCPConnection::~TCPConnection()
{

}

void TCPConnection::DoSend( const boost::asio::const_buffers_1& buffer )
{
	boost::system::error_code error;
	boost::asio::write( m_socket, buffer, boost::asio::transfer_all(), error );
	if( error )
		throw NetworkException( error );
}

void TCPConnection::DoRecv( boost::asio::mutable_buffers_1 buffer, size_t recvSize )
{
	//TODO: Add a way to make sure we don't read too much stuff here
	//		internal buffering perhaps?
	boost::system::error_code error;
	size_t totalLen = 0;
	do
	{
		//TODO: Make sure that this doesn't overwrite the current contents each time it's called
		size_t len = m_socket.read_some( buffer, error );
		totalLen += len;
		if( error )
			break;
	}
	while( totalLen < recvSize );

	if( error )
		throw NetworkException( error );
}

}	// namespace net
}	// namespace kode
