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

#ifndef KODE_DB_UTILS_H
#define KODE_DB_UTILS_H_

#include <string>

namespace kode {
namespace db {
namespace utils {

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	Utility function to check sqlite error codes
//!	
//!	\param	errorCode		The error code to check
//!	\param	errorLocation	A string describing the location this check
//!							is taking place
//!
//!	\throws	A SqlException if an error has occurred
//!
///////////////////////////////////////////////////////////////////////////////
void CheckErrorCode( 
	int errorCode,
	const std::string& errorLocation
	);

}	// namespace utils
}	// namespace db
} 	// namespace kode

#endif
