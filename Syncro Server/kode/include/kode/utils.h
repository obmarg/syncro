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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <boost/detail/endian.hpp>
#include <boost/foreach.hpp>

namespace kode
{
namespace utils
{

#define foreach BOOST_FOREACH

class CStringLessThan
{
public:
	bool operator()( const std::string& inoLHS, const std::string& inoRHS ) const
	{
		return ( inoLHS.compare( inoRHS ) < 0 );
	}
};

class CPointerLessThan
{
public:
	template<class tPointerType>
	bool operator()( const tPointerType& inoLHS, const tPointerType& inoRHS ) const
	{
		return (( *inoLHS ) < ( *inoRHS ) );
	}
};

template<typename T>
T ToJavaEndian( const T& source )
{
#ifdef BOOST_BIG_ENDIAN
	return source;
#else
	T ret = 0;

	for( int i = 0; i < sizeof( T ); ++i )
	{
		*( reinterpret_cast<char*>( &ret ) + i ) =
		    *( reinterpret_cast<const char*>( &source ) + sizeof( T ) - i - 1 );
	}
	return ret;
#endif
}

template<typename T>
T FromJavaEndian( const T& source )
{
#ifdef BOOST_BIG_ENDIAN
	return source;
#else
	T ret = 0;

	for( int i = 0; i < sizeof( T ); ++i )
	{
		*( reinterpret_cast<char*>( &ret ) + i ) =
		    *( reinterpret_cast<const char*>( &source ) + sizeof( T ) - i - 1 );
	}
	return ret;
#endif
}

extern std::string GenerateUUID();
extern std::wstring wstring( const std::string& str );
extern std::string string( const std::wstring& str );

#ifdef _WIN32
const std::string PATH_SEPERATOR_STR = "\\";
const char PATH_SEPERATOR_CHAR = '\\';
#else
const std::string PATH_SEPERATOR_STR = "/";
const char PATH_SEPERATOR_CHAR = '/';
#endif

}	// namespace utils
};	// namespace kode

#endif
