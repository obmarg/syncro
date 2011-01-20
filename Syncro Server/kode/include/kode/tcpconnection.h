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
