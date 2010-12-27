#include <libsyncro/stringutils.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>

namespace syncro {
namespace utils {

void ReplaceStringVars(std::string& string)
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