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

namespace syncro
{

class CSyncroDB : public kode::db::Database, boost::noncopyable
{
public:
	static Database::TPointer OpenDB()
	{
		Database::TPointer pDatabase( new CSyncroDB( "syncro.db" ) );
		return pDatabase;
	}

	virtual ~CSyncroDB();
private:
	static const int EXPECTED_DB_VERSION;

	CSyncroDB( std::string insFile );

	bool CreateDatabase();
	bool UpgradeDatabase( int nCurrentVersion );
};

};		//namespace syncro

#endif