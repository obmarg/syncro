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

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	An exception representing a network error
//!
///////////////////////////////////////////////////////////////////////////////
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