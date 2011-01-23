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

#include "utils.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <vector>

namespace kode
{
namespace utils
{

std::string GenerateUUID()
{
	std::vector<unsigned char> aUUID( 16 );
	boost::uuids::random_generator oGenerator;
	boost::uuids::uuid oUUID( oGenerator() );
	if( oUUID.size() != 16 )
		throw std::runtime_error( "Invalid legnth of UUID generated in CHandshakeResponse" );
	for( boost::uuids::uuid::size_type n = 0; n < oUUID.size(); n++ )
	{
		aUUID[n] = oUUID.data[n];
	}
	return std::string( aUUID.begin(), aUUID.end() );
}

std::wstring wstring( const std::string& str )
{
	std::wstring rv( str.length(), 0 );
	std::string::const_iterator
	srcIt = str.begin(),
	srcItEnd = str.end();
	std::wstring::iterator
	destIt = rv.begin(),
	destItEnd = rv.end();
	for( ; srcIt != srcItEnd; ++srcIt, ++destIt )
	{
		( *destIt ) = boost::numeric_cast<wchar_t>( *srcIt );
	}
	return rv;
}

std::string string( const std::wstring& str )
{
	std::string rv( str.length(), 0 );
	std::wstring::const_iterator
	srcIt = str.begin(),
	srcItEnd = str.end();
	std::string::iterator
	destIt = rv.begin(),
	destItEnd = rv.end();
	for( ; srcIt != srcItEnd; ++srcIt, ++destIt )
	{
		( *destIt ) = boost::numeric_cast<char>( *srcIt );
	}
	return rv;
}

}	// namespace utils
}	// namespace kode
