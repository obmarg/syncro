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

#ifndef _KODE_BASE64_H_
#define _KODE_BASE64_H_

#include <cryptopp/base64.h>
#include <boost/numeric/conversion/cast.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <stdint.h>

namespace kode
{
namespace base64
{

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	Encodes some bytes into base64
//!
//!	\param	data	The data buffer to encode
//!	\param	len		The legnth of the data
//!
//!	\return	A base64 encoded string containing the data
//!
///////////////////////////////////////////////////////////////////////////////
inline std::string Encode( 
	const unsigned char* data, 
	const unsigned int len 
	)
{
	CryptoPP::Base64Encoder encoder;

	encoder.PutMessageEnd(
	    data,
	    len
	);

	if( !encoder.AnyRetrievable() )
		throw std::runtime_error( "Base64 error in kode::base64::Encode" );
	uint32_t destSize = boost::numeric_cast< uint32_t >(
	                        encoder.MaxRetrievable()
	                    );
	std::vector< char > destVector( destSize );

	unsigned int sizeRead = encoder.Get(
	                            reinterpret_cast< unsigned char* >( &destVector[0] ),
	                            destSize
	                        );
	if( sizeRead != destSize )
	{
		std::cout << "Warning: Base64 read length is less than expected in " <<
		          "kode::base64::Encode()\n";
		destVector.resize( sizeRead );
	}

	return std::string( destVector.begin(), destVector.end() );
}

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	Decodes a string from base64
//!
//!	\param	input	The string to decode
//!	\param	output	A vector to write the decoded data to
//!
///////////////////////////////////////////////////////////////////////////////
inline void Decode( 
	const std::string& input, 
	std::vector< unsigned char >& output 
	)
{
	CryptoPP::Base64Decoder decoder;

	decoder.PutMessageEnd(
	    reinterpret_cast< const unsigned char* >( input.c_str() ),
	    input.length()
	);

	if( !decoder.AnyRetrievable() )
		throw std::runtime_error( "Base64 error in kode::base64::Decode" );

	uint32_t destSize =  boost::numeric_cast< uint32_t >(
	                         decoder.MaxRetrievable()
	                     );
	if( destSize == 0 )
		throw std::runtime_error( "Base64 error in kode::base64::Decode" );
	output.resize( destSize );

	unsigned int sizeRead = decoder.Get( &output[0], destSize );
	if( sizeRead != destSize )
	{
		std::cout << "Warning: Base64 read length is less than expected in " <<
		          "kode::base64::Encode()\n";
		output.resize( sizeRead );
	}

}

}	// namespace base64
}	// namespace kode

#endif
