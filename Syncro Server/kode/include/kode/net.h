#ifndef _KODE_NET_H_
#define _KODE_NET_H_

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <string>
#include <exception>

namespace kode
{
namespace net
{

class NetworkException : public std::runtime_error
{
public:
	NetworkException( boost::system::error_code error ) :
		std::runtime_error(
		    boost::system::system_error( error ).what()
		)
	{
	};
	NetworkException( const char* desc ) : std::runtime_error( desc ) {};
	NetworkException( const std::string& desc ) : std::runtime_error( desc.c_str() ) {};
	NetworkException();
};

}	// namespace kode
}	// namespace net

#endif