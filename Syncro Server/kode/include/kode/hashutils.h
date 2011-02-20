/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	This file defines various classes/functions for hashing

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

#ifndef KODE_HASHUTILS_H_
#define KODE_HASHUTILS_H_

#ifndef KODE_REMOVE_HASHUTILS

#include <boost/shared_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace kode {

class BaseHash
{
public:
	virtual unsigned int			Size() const = 0;
	virtual const unsigned char*	GetHash()		const = 0;

	bool operator==( const BaseHash& other ) const
	{
		return Compare( other );
	}
protected:
	virtual bool Compare( const BaseHash& other ) const = 0;
};

typedef const boost::shared_ptr<BaseHash> HashPtr;

template<class tHash>
class Hash : public BaseHash
{
public:
	virtual unsigned int Size( ) const
	{ return sizeof( m_hash ); }

	virtual const unsigned char* GetHash() const
	{ return &m_hash[0]; }

	virtual bool Compare( const BaseHash& other ) const
	{
		const Hash<tHash>& otherHash = 
			dynamic_cast< const Hash<tHash>& >( other );
		return memcmp( m_hash, otherHash.m_hash, sizeof( m_hash ) ) == 0;
	};

	unsigned char m_hash[ tHash::DIGESTSIZE ];
};

template<class tHash>
HashPtr HashFile( const std::string& filename, unsigned int sizeToHash=0 )
{

	tHash hashObject;
	std::ifstream file(
		filename,
		std::ios::binary | std::ios::in
		);
	if( file.fail() )
	{
		throw std::runtime_error( 
			"Could not open input file in kode::HashFile" 
			);
	}
	file.seekg( 0, std::ios::end );
	int64_t fileSize = file.tellg();
	file.seekg( 0, std::ios::beg );

	if( sizeToHash != 0 )
	{
		fileSize = sizeToHash;
	}

	std::vector<unsigned char> buffer( 1024 );
	
	do
	{
		int64_t sizeLeft = fileSize - file.tellg();
		if( sizeLeft == 0 )
			break;

		if( sizeLeft < buffer.size() )
			buffer.resize(
			boost::numeric_cast< size_t >( sizeLeft )
			);
		file.read(
			reinterpret_cast< char* >( &buffer[0] ),
			buffer.size()
			);

		hashObject.Update( &buffer[0], buffer.size() );
	}
	while( !file.eof() );

	boost::shared_ptr<BaseHash> rv( new Hash<tHash>() );

	hashObject.Final( dynamic_cast< Hash<tHash>* >( rv.get() )->m_hash );

	return rv;
}

}	// namespace kode

#endif	// KODE_REMOVE_HASHUTILS

#endif	// KODE_HASHUTILS_H_