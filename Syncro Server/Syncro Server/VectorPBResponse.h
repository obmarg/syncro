#ifndef _VECTOR_PB_RESPONSE_H_
#define _VECTOR_PB_RESPONSE_H_

#include "common.h"
#include "BasePBResponse.h"

namespace syncro {

class CVectorPBResponse : public CBasePBResponse {
public:

	virtual std::vector<unsigned int> GetSubpacketSizes() {
		std::vector<unsigned int> oResponse;
		foreach( TSubpacketPtr pSubpacket, m_aSubpackets ) {
			oResponse.push_back( pSubpacket->size() );
		}
		return oResponse;
	};
	virtual unsigned int GetSubpacketCount() {
		return m_aSubpackets.size();
	};

	//TODO: Write all this stuff to use ArrayOutputStreams?
	virtual void WriteSubpacket(int inSubpacketIndex,std::back_insert_iterator<TCharBuffer::TBuff> inoInsert) {
		std::copy( m_aSubpackets[inSubpacketIndex]->begin(),m_aSubpackets[inSubpacketIndex]->end(), inoInsert );
	}
	
protected:
	CVectorPBResponse() {};
	~CVectorPBResponse() {};

	typedef boost::shared_ptr<TCharBuffer::TBuff> TSubpacketPtr;
	typedef std::vector<TSubpacketPtr> TSubpacketList;
	TSubpacketList m_aSubpackets;
};

};		//namespace syncro

#endif