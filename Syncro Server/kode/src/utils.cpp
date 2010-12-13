#include "utils.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>

namespace kode {
namespace utils {

std::string GenerateUUID()
{
	std::vector<unsigned char> aUUID( 16 );
	boost::uuids::random_generator oGenerator;
	boost::uuids::uuid oUUID( oGenerator() );
	if( oUUID.size() != 16 )
		throw std::runtime_error( "Invalid legnth of UUID generated in CHandshakeResponse" );
	for( boost::uuids::uuid::size_type n=0; n<oUUID.size(); n++ ) {
		aUUID[n] = oUUID.data[n];
	}
	return std::string( aUUID.begin(), aUUID.end() );
}

}	// namespace utils
}	// namespace kode
