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

#ifndef _VECTOR_PB_RESPONSE_H_
#define _VECTOR_PB_RESPONSE_H_

#include "common.h"
#include "BasePBResponse.h"

namespace syncro {
namespace pbHandlers {

class CVectorPBResponse : public BasePBResponse
{
public:

	virtual uint32_t GetSubpacketSize(uint32_t subpacket);
	virtual unsigned int GetSubpacketCount();

	virtual void WriteSubpacket( 
		int inSubpacketIndex, 
		google::protobuf::io::ZeroCopyOutputStream& stream 
		);

protected:
	CVectorPBResponse() {};
	~CVectorPBResponse() {};

	typedef boost::shared_ptr<TCharBuffer::TBuff> TSubpacketPtr;
	typedef std::vector<TSubpacketPtr> TSubpacketList;
	TSubpacketList m_aSubpackets;
};

}	// namespace pbHandlers
}	// namespace syncro

#endif