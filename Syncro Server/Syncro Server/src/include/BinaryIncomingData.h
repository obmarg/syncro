#ifndef _BINARY_INCOMING_DATA_H_
#define _BINARY_INCOMING_DATA_H_

#include "common.h"
#include <kode/stopwatch.h>
#include <fstream>

namespace syncro {

class CBinaryIncomingData {
public:
	CBinaryIncomingData( 
		const std::string sFilename, 
		VoidCallback completedCallback 
		);
	~CBinaryIncomingData();

	void HandlePacket(TInputStreamList& inaInputStreams);
private:
	std::ofstream m_oFile;
	VoidCallback m_completedCallback;
	kode::time::Stopwatch m_stopwatch;
	unsigned int m_dataTransferred;
};

}

#endif