#ifndef KODE_STOPWATCH_H_
#define KODE_STOPWATCH_H_

#include <boost/date_time/posix_time/posix_time.hpp>

namespace kode
{
namespace time
{

class Stopwatch
{
public:
	Stopwatch()
		: m_running( false )
	{
		Start();
	}

	void Start();
	void Reset();
	void Stop();

	int64_t GetMS();
private:
	boost::posix_time::ptime m_startTime;
	boost::posix_time::ptime m_finishTime;
	bool m_running;
};

}	// namespace time
}	// namespace kode

#endif