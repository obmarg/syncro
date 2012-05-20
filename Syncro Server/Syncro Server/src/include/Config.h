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

	This file contains a simple class for dealing with config values
*/

#ifndef SYNCRO_CONFIG_H_
#define SYNCRO_CONIFG_H_

#include <boost/program_options.hpp>

namespace syncro {

namespace po = boost::program_options;

////////////////////////////////////////////////////////////////////////
//!
//! \brief  Simple singleton class that deals with configuration
//!
////////////////////////////////////////////////////////////////////////
class Config
{
public:
    static Config& GetInstance();
    
    po::options_description GetOptionsDescription();
    void Init( const po::variables_map& vars );

    std::string TempFilesPath()
    { return m_tempFilesPath; }

private:
    Config();

private:
    std::string m_tempFilesPath;
};

}   // namespace syncro

#endif  // SYNCRO_CONFIG_H_

