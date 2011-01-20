#include "stopwatch.h"

namespace kode
{
namespace time
{

void Stopwatch::Start()
{
	if( !m_running )
		Reset();
}

void Stopwatch::Reset()
{
	m_startTime =
	    boost::posix_time::microsec_clock::universal_time();
	m_running = true;
}

void Stopwatch::Stop()
{
	if( m_running )
	{
		m_finishTime =
		    boost::posix_time::microsec_clock::universal_time();
		m_running = false;
	}
}

int64_t Stopwatch::GetMS()
{
	boost::posix_time::time_duration duration;
	if( m_running )
	{
		duration =
		    boost::posix_time::microsec_clock::universal_time()
		    - m_startTime
		    ;
	}
	else
	{
		duration = m_finishTime - m_startTime;
	}
	return duration.total_milliseconds();
}

}	// namespace time
}	// namespace kode
