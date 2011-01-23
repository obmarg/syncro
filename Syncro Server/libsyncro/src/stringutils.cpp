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

#include <libsyncro/stringutils.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>

namespace syncro
{
namespace utils
{

void ReplaceStringVars( std::string& string )
{
	using boost::regex;
	using boost::lexical_cast;

	static const regex monthText( "%montht%" );
	static const regex monthNumber( "%monthn%" );
	static const regex year( "%year%" );

	boost::posix_time::ptime time(
	    boost::posix_time::microsec_clock::local_time()
	);

	string = boost::regex_replace(
	             string,
	             monthText,
	             time.date().month().as_short_string(),
	             boost::match_default | boost::format_perl
	         );
	string = boost::regex_replace(
	             string,
	             monthNumber,
	             lexical_cast< std::string >(
	                 time.date().month().as_number()
	             ),
	             boost::match_default | boost::format_perl
	         );
	string = boost::regex_replace(
	             string,
	             year,
	             lexical_cast< std::string >(
	                 time.date().year()
	             ),
	             boost::match_default | boost::format_perl
	         );
}

}	// namespace utils
}	// namespace syncro