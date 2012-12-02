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

#ifndef SYNCRO_LOGGING_H_
#define SYNCRO_LOGGING_H_ 

#include "kode/logger.h"

namespace syncro {
namespace log {

extern kode::Logger g_log;

extern kode::LogLevel error;
extern kode::LogLevel warning;
extern kode::LogLevel info;
extern kode::LogLevel debug;

}   // namespace log
}   // namespace syncro

#endif  // SYNCRO_LOGGING_H_

