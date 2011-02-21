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

#include "kode/stopwatch.h"

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
