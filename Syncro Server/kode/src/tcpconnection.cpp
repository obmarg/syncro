#include "tcpconnection.h"
#include <boost/lexical_cast.hpp>

namespace kode {
namespace net {

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
	while( error && iter != endIter ) {
		m_socket.close();
		m_socket.connect(*iter, error );
		++iter;
	}
	if( error ) {
		throw boost::system::system_error( error );
	}
}

TCPConnection::~TCPConnection() {

}

void TCPConnection::DoSend( const boost::asio::const_buffers_1& buffer ) {
	boost::system::error_code error;
	boost::asio::write( m_socket, buffer, boost::asio::transfer_all(), error );
	if( error )
		throw boost::system::system_error( error );
}

void TCPConnection::DoRecv( boost::asio::mutable_buffers_1& buffer, size_t recvSize ) {
	//TODO: Add a way to make sure we don't read too much stuff here
	//		internal buffering perhaps?
	boost::system::error_code error;
	size_t totalLen = 0;
	do {
		size_t len = m_socket.read_some( buffer, error );
		totalLen += len;
	}while( totalLen < recvSize );

	if( error )
		throw boost::system::system_error( error );
}

}	// namespace net
}	// namespace kode