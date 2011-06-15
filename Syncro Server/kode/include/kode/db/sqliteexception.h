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

#ifndef KODE_DB_SQLITEEXCEPTION_H_
#define KODE_DB_SQLITEEXCEPTION_H_

#include <stdexcept>
#include <string>

namespace kode {
namespace db {

class SqlException : std::runtime_error
{
public:
	SqlException( std::string sMessage, int nErrorCode ) : runtime_error( sMessage.c_str() )
	{
		m_nErrorCode = nErrorCode;
	}
private:
	int m_nErrorCode;
};

}	// namespace db 
} 	// namespace kode 

#endif 	// KODE_DB_SQLITEEXCEPTION_H_
