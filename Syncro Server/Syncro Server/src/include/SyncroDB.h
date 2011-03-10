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

#ifndef __SYNCRO_DB_H_
#define __SYNCRO_DB_H_

#include <kode/Database.h>
#include <string>

namespace syncro
{

class SyncroDB : public kode::db::Database, boost::noncopyable
{
public:
	static Database::TPointer OpenDB(
		const std::string& file=DEFAULT_DB_NAME
		)
	{
		Database::TPointer pDatabase( new SyncroDB( file ) );
		return pDatabase;
	}

	virtual ~SyncroDB();

public:
	static void	SetDefaultFilename( const std::string& name )
	{ DEFAULT_DB_NAME = name; }

private:
	static const int	EXPECTED_DB_VERSION;
	static std::string	DEFAULT_DB_NAME;

	SyncroDB( const std::string& insFile );

	bool CreateDatabase();
	bool UpgradeDatabase( int nCurrentVersion );
};

};		//namespace syncro

#endif