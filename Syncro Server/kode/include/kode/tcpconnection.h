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

#ifndef _KODE_NET_TCPCONNECTION_H_
#define _KODE_NET_TCPCONNECTION_H_

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <string>

namespace kode
{
namespace net
{

class TCPConnection : boost::noncopyable
{
public:
	TCPConnection( std::string hostname, int nPort );
	virtual ~TCPConnection();

	template< class tBuffer >
	void Send( const tBuffer& buffer )
	{
		DoSend( boost::asio::buffer( buffer ) );
	}

	template< class tBuffer >
	void Send( const tBuffer& buffer, size_t size )
	{
		DoSend( boost::asio::buffer( buffer, size ) );
	}

	template< class tBuffer >
	void SendByVal( const tBuffer buffer )
	{
		DoSend( boost::asio::buffer( buffer ) );
	}

	template< class tBuffer >
	void SendByVal( const tBuffer buffer, size_t size )
	{
		DoSend( boost::asio::buffer( buffer, size ) );
	}

	template< class tBuffer >
	void Recv( tBuffer& buffer, size_t recvSize )
	{
		DoRecv( boost::asio::buffer( buffer, recvSize ), recvSize );
	}

	template< class tBuffer >
	void RecvByVal( tBuffer buffer, size_t recvSize )
	{
		DoRecv( boost::asio::buffer( buffer, recvSize ), recvSize );
	}

protected:
	boost::asio::io_service			m_service;
	boost::asio::ip::tcp::socket	m_socket;

private:
	void DoSend( const boost::asio::const_buffers_1& buffer );
	void DoRecv( boost::asio::mutable_buffers_1 buffer, size_t recvSize );
};

}	// namespace net
}	// namespace kode

#endif
