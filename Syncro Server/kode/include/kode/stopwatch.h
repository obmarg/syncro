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

#ifndef KODE_STOPWATCH_H_
#define KODE_STOPWATCH_H_

#include <boost/date_time/posix_time/posix_time.hpp>

namespace kode {
namespace time {

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	A class implementing a basic stopwatch style timer
//!
///////////////////////////////////////////////////////////////////////////////
class Stopwatch
{
public:
	Stopwatch()
		: m_running( false )
	{
		Start();
	}

	//!	\brief Start the stopwatch
	void Start();

	//!	\brief Reset the stopwatch
	void Reset();

	//!	\brief Stop the stopwatch
	void Stop();

	//!
	//!	\brief Get the current reading
	//!
	//!	\return	The number of milliseconds the stopwatch has been running
	//!			since it was last reset
	//!
	int64_t GetMS();
private:
	boost::posix_time::ptime m_startTime;
	boost::posix_time::ptime m_finishTime;
	bool m_running;
};

}	// namespace time
}	// namespace kode

#endif