#ifndef _KODE_BASE64_H_
#define _KODE_BASE64_H_

#include <cryptopp/base64.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace kode {
namespace base64 {

std::string Encode(const unsigned char* data,const unsigned int len)
{
	CryptoPP::Base64Encoder encoder;
	
	encoder.PutMessageEnd(
		data,
		len
		);

	if( !encoder.AnyRetrievable() )
		throw std::runtime_error( "Base64 error in kode::base64::Encode" );

	unsigned int destSize = encoder.MaxRetrievable();
	std::vector< char > destVector( destSize );

	unsigned int sizeRead = encoder.Get( 
		reinterpret_cast< unsigned char* >( &destVector[0] ),
		destSize
		);
	if( sizeRead != destSize )
	{
		std::cout << "Warning: Base64 read legnth is less than expected in " <<
			"kode::base64::Encode()\n";
		destVector.resize( sizeRead );
	}

	return std::string( destVector.begin(), destVector.end() );
}

void Decode( const std::string& input, std::vector< unsigned char >& output )
{
	CryptoPP::Base64Decoder decoder;

	decoder.PutMessageEnd(
		reinterpret_cast< const unsigned char* >( input.c_str() ),
		input.length()
		);

	if( !decoder.AnyRetrievable() )
		throw std::runtime_error( "Base64 error in kode::base64::Decode" );

	unsigned int destSize = decoder.MaxRetrievable();
	if( destSize == 0 )
		throw std::runtime_error( "Base64 error in kode::base64::Decode" );
	output.resize( destSize );
	
	unsigned int sizeRead = decoder.Get( &output[0], destSize );
	if( sizeRead != destSize )
	{
		std::cout << "Warning: Base64 read legnth is less than expected in " <<
			"kode::base64::Encode()\n";
		output.resize( sizeRead );
	}

}

}	// namespace base64
}	// namespace kode

#endif
