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
	virtual void WriteSubpacket(int inSubpacketIndex,google::protobuf::io::ZeroCopyOutputStream& stream) {
		//std::copy( m_aSubpackets[inSubpacketIndex]->begin(),m_aSubpackets[inSubpacketIndex]->end(), inoInsert );
		int nToCopy = m_aSubpackets[inSubpacketIndex]->size();
		void* pData;
		int nCurrentBufferSize = 0;
		TCharBuffer::TBuff::iterator pEnd = m_aSubpackets[inSubpacketIndex]->end();
		TCharBuffer::TBuff::iterator pCurr = m_aSubpackets[inSubpacketIndex]->begin();
		while( (pCurr != pEnd) ) {
			if( !stream.Next(&pData, &nCurrentBufferSize ) ) {
				throw std::runtime_error( "ZeroCopyOutputStream::Next returned false in VectorPBResponse::WriteSubpacket" );
			}
			TCharBuffer::TChar* pCurrBuffer = reinterpret_cast< TCharBuffer::TChar* >( pData );
			if( nCurrentBufferSize != 0 ) {
				if( nCurrentBufferSize > nToCopy )
					nCurrentBufferSize = nToCopy;
				TCharBuffer::TBuff::iterator pNextBlock = pCurr + nCurrentBufferSize;
				//TODO: memcpy or something might be faster than std::copy here
				//		also might not output a call parameters may be unsafe error
				std::copy( pCurr, pNextBlock, pCurrBuffer );
				pCurr = pNextBlock;
				nToCopy -= nCurrentBufferSize;
			}
		}
		//TODO: Handle errors?
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