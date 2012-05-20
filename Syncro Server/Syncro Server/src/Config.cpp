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

#include "Config.h"

namespace syncro {

static const std::string DefaultTempDir = "~/.syncro/temp";

////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor.  Doesn't do much, other than set some default
//!         values
//!
////////////////////////////////////////////////////////////////////////
Config::Config() :
m_tempFilesPath( DefaultTempDir )
{

}

////////////////////////////////////////////////////////////////////////
//!
//! \brief  Singleton access function
//!
////////////////////////////////////////////////////////////////////////
Config& Config::GetInstance()
{
    static Config instance;
    return instance;
}

////////////////////////////////////////////////////////////////////////
//!
//! \brief  Function to get the options description for parsing
//!         options
//!
////////////////////////////////////////////////////////////////////////
po::options_description Config::GetOptionsDescription()
{
    po::options_description desc("Configuration options");
    desc.add_options()
        ( 
            "tempdir", 
            po::value< std::string>(), 
            "Directory for temporary files" 
            );
    return desc;
}

////////////////////////////////////////////////////////////////////////
//!
//! \brief  Function that initialises the config class with parsed
//!         options
//!
////////////////////////////////////////////////////////////////////////
void Config::Init( const po::variables_map& vars )
{
    if ( vars.count( "tempdir" ) )
    {
        m_tempFilesPath = vars[ "tempdir" ].as< std::string >();
    }
}

}   // namespace syncro
