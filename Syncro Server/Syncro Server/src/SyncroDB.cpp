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

#ifndef SYNCRO_SERVER
#include "../../Syncro Server/src/include/SyncroDB.h"
#else
#include "SyncroDB.h"
#endif

namespace syncro
{

const int	SyncroDB::EXPECTED_DB_VERSION	= 12;
std::string	SyncroDB::DEFAULT_DB_NAME		= "syncro.db";

const std::string FOLDERS_TABLE_NAME = "Folders";
const std::string FOLDERS_TABLE_CREATE =
    "CREATE TABLE IF NOT EXISTS " + FOLDERS_TABLE_NAME + " (" +
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
    "Name TEXT NOT NULL, " +
    "Path TEXT NOT NULL, " +
    "Writable INTEGER NOT NULL DEFAULT 0, " +
    "Readable INTEGER NOT NULL DEFAULT 1, " +
    "UploadPrefix TEXT NOT NULL DEFAULT '');";

const std::string SERVER_ID_TABLE_NAME = "ServerID";
const std::string SERVER_ID_TABLE_CREATE =
    "CREATE TABLE IF NOT EXISTS " + SERVER_ID_TABLE_NAME + " (" +
    "uuid TEXT NOT NULL," +
    "servername TEXT NOT NULL);";

const std::string USERS_TABLE_NAME = "Users";
const std::string USERS_TABLE_CREATE =
    "CREATE TABLE IF NOT EXISTS " + USERS_TABLE_NAME + " (" +
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
    "Username TEXT NOT NULL, " +
    "Password TEXT NOT NULL, " +
    "IP TEXT NOT NULL);";

const std::string FILES_TABLE_NAME = "Files";
const std::string FILES_TABLE_CREATE =
    "CREATE TABLE IF NOT EXISTS " + FILES_TABLE_NAME + " (" +
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
    "Filename TEXT NOT NULL, " +
    "FolderPath TEXT NOT NULL, " +
    "LocalPath TEXT NOT NULL, " +
    "OneShot INTEGER NOT NULL, "
    "FolderID INTEGER NOT NULL, "
	"DeleteOnFinish INTEGER NOT NULL DEFAULT 0);";

const std::string FILES_TABLE_UPGRADE_V11_V12 =
	"ALTER TABLE " + FILES_TABLE_NAME +
	" ADD COLUMN DeleteOnFinish INTEGER NOT NULL DEFAULT 0;";

const std::string UPLOAD_HISTORY_TABLE_NAME = "UploadHistory";
const std::string UPLOAD_HISTORY_CREATE =
    "CREATE TABLE IF NOT EXISTS " + UPLOAD_HISTORY_TABLE_NAME + " (" +
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
    "Filename TEXT NOT NULL, " +
    "FolderID INTEGER NOT NULL, " +
    "ActualFilename TEXT NOT NULL);";

const std::string DEFAULT_USER_CREATE =
    "INSERT INTO " + USERS_TABLE_NAME + " (" +
    "Username,Password,IP ) " +
    " VALUES " +
    "('admin','W6ph5Mm5Pz8GgiULbPgzG37mj9g=','127.0.0.1');";

//TODO: Move the database versioning stuff out into kode probably, then subclass that here.

SyncroDB::SyncroDB( 
	const std::string& insFilename 
	) : 
kode::db::SqliteDatabase( insFilename )
{
	int nCurrentVersion = runScalar<int>( "PRAGMA user_version;" );

	if( nCurrentVersion != EXPECTED_DB_VERSION )
	{
		bool fOK;
		if( nCurrentVersion == 0 )
		{
			fOK = CreateDatabase();
		}
		else
		{
			fOK = UpgradeDatabase( nCurrentVersion );
		}
		if( fOK )
		{
			runInsert( 
				"PRAGMA user_version=" + 
				boost::lexical_cast<std::string>( EXPECTED_DB_VERSION ) + 
				";" 
				);
		}
	}
}

SyncroDB::~SyncroDB()
{

}

bool SyncroDB::CreateDatabase()
{
	runInsert( FOLDERS_TABLE_CREATE );
	runInsert( SERVER_ID_TABLE_CREATE );
	runInsert( USERS_TABLE_CREATE );
	runInsert( FILES_TABLE_CREATE );
	runInsert( UPLOAD_HISTORY_CREATE );
	runInsert( DEFAULT_USER_CREATE );
	clearResult();
	return true;
}

bool SyncroDB::UpgradeDatabase( int nCurrentVersion )
{
	if( nCurrentVersion == 1 )
		runInsert( SERVER_ID_TABLE_CREATE );
	if( nCurrentVersion == 2 )
	{
		runInsert( "DROP TABLE " + SERVER_ID_TABLE_NAME + ";" );
		runInsert( SERVER_ID_TABLE_CREATE );
	}
	if( nCurrentVersion < 5 )
	{
		runInsert( USERS_TABLE_CREATE );
	}
	if( nCurrentVersion < 6 )
	{
		runInsert( "DROP TABLE " + USERS_TABLE_NAME + ";" );
		runInsert( USERS_TABLE_CREATE );
	}
	if( nCurrentVersion < 7 )
	{
		runInsert( FILES_TABLE_CREATE );
	}
	if( nCurrentVersion < 8 )
	{
		runInsert( "DROP TABLE " + FOLDERS_TABLE_NAME + ";" );
		runInsert( FOLDERS_TABLE_CREATE );
	}
	if( nCurrentVersion < 9 )
	{
		runInsert( UPLOAD_HISTORY_CREATE );
	}
	if( nCurrentVersion < 11 )
	{
		runInsert( "DELETE FROM " + USERS_TABLE_NAME + ";" );
		runInsert( DEFAULT_USER_CREATE );
	}
	if( nCurrentVersion < 12 )
	{
		runInsert( FILES_TABLE_UPGRADE_V11_V12 );
	}
	clearResult();
	return true;
}

};		//namespace syncro
