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

#ifndef KODE_LOGGER_H_
#define KODE_LOGGER_H_

#include <utility>
#include <iostream>

namespace kode {

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	A simple logging class.
//!	        Currently just logs to stdout.
//!
///////////////////////////////////////////////////////////////////////////////
class Logger
{
public:
    Logger() :
    m_currentLevel( 1 )
    {
    }

    template< class T >
    void Log( T&& message, int level )
    {
        if ( level <= m_currentLevel )
        {
            std::cout << std::forward<T>( message );
        }
    }

    void SetLevel( int level )
    {
        m_currentLevel = level;
    }

private:
    int m_currentLevel;
};

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	A class that represents a log level.
//!
///////////////////////////////////////////////////////////////////////////////
class LogLevel
{
public:
    LogLevel( Logger& log, int level ) :
    m_log( log ),
    m_level( level )
    {}

    template< class T >
    LogLevel& operator<<( T&& message )
    {
        m_log.Log( std::forward<T>( message ), m_level );
        return (*this);
    }

private:
    Logger& m_log;
    int m_level;
};

}   // namespace kode

#endif  // KODE_LOGGER_H_
