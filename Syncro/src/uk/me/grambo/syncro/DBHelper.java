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

package uk.me.grambo.syncro;

import android.database.sqlite.*;
import android.content.*;

public class DBHelper extends SQLiteOpenHelper {

    private static final int DATABASE_VERSION = 19;
    private static final String DATABASE_NAME="SyncroDB";
    private static final String SERVERS_TABLE_NAME = "servers";
    private static final String SERVERS_TABLE_CREATE =
                "CREATE TABLE IF NOT EXISTS " + SERVERS_TABLE_NAME + " (" +
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
                "Name TEXT, " +
                "IP TEXT, " + 
                "Port INTEGER, " +
                "Username TEXT, " + 
                "Password TEXT, " +
                "CONSTRAINT IP_NAME_UNIQUE UNIQUE (Name,IP) ON CONFLICT FAIL);";
    
    private static final String SERVERS_TABLE_UPGRADE_V16_V17 =
    			"ALTER TABLE " + SERVERS_TABLE_NAME + 
    			" ADD COLUMN Username TEXT;\n" +
    			"ALTER TABLE " + SERVERS_TABLE_NAME + 
    			" ADD COLUMN Password TEXT;\n";
    
    private static final String FOLDERS_TABLE_NAME = "folders";
    private static final String FOLDERS_TABLE_CREATE = 
    			"CREATE TABLE IF NOT EXISTS " + FOLDERS_TABLE_NAME + " (" +
    			"ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
    			"ServerID INTEGER CONSTRAINT FOLDERS_SERVER_ID_FK REFERENCES " + SERVERS_TABLE_NAME + "(ID) ON DELETE CASCADE ON UPDATE CASCADE, " +
    			"IDOnServer INTEGER NOT NULL, " +
    			"Name TEXT, " +
    			"ServerPath TEXT, " +
    			"LocalPath TEXT," +
    			"SyncToPhone INTEGER CONSTRAINT SYNC_TO_PHONE_DEFAULT DEFAULT 0, " + 
    			"SyncFromPhone INTEGER CONSTRAINT SYNC_FROM_PHONE_DEFAULT DEFAULT 0," +
    			"LastSyncTime INTEGER NOT NULL DEFAULT 0, " +
    			"Deleted INTEGER NOT NULL DEFAULT 0, " +
    			"CONSTRAINT PK_FOLDERS UNIQUE (IDOnServer,ServerID) ON CONFLICT IGNORE);";
    
    private static final String FOLDERS_TABLE_UPGRADE_V16_V17 =
    			"ALTER TABLE " + FOLDERS_TABLE_NAME +
    			" ADD COLUMN LastSyncTime INTEGER NOT NULL DEFAULT 0;";
    
    private static final String FOLDERS_TABLE_UPGRADE_V17_V18 =
    			"ALTER TABLE " + FOLDERS_TABLE_NAME +
    			" ADD COLUMN Deleted INTEGER NOT NULL DEFAULT 0;";
    
    private static final String FILTERS_TABLE_NAME = "filters";
    private static final String FILTERS_TABLE_CREATE =
    			"CREATE TABLE IF NOT EXISTS " + FILTERS_TABLE_NAME + " (" +
    			"ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
    			"FolderID INTEGER CONSTRAINT FILTERS_FOLDER_ID_FK REFERENCES " + FOLDERS_TABLE_NAME + "(ID) ON DELETE CASCADE ON UPDATE CASCADE, " + 
    			"FilterType INTEGER NOT NULL, " + 
    			"Name TEXT," +
    			"IncludeType INTEGER NOT NULL DEFAULT 0," +
    			"FilenameType INTEGER NOT NULL DEFAULT 0);";
    
    private static final String UPLOADS_TABLE_NAME = "uploads";
    private static final String UPLOADS_TABLE_CREATE =
    			"CREATE TABLE IF NOT EXISTS " + UPLOADS_TABLE_NAME + " (" +
    			"ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
    			"FolderID INTEGER CONSTRAINT UPLOADS_FOLDER_ID_FK REFERENCES " + FOLDERS_TABLE_NAME + "(ID) ON DELETE CASCADE ON UPDATE CASCADE, " +
    			"Filename TEXT NOT NULL);";
    

    public DBHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(SERVERS_TABLE_CREATE);
        db.execSQL(FOLDERS_TABLE_CREATE);
        db.execSQL(FILTERS_TABLE_CREATE);
        db.execSQL(UPLOADS_TABLE_CREATE);
    }
    
    @Override
    public void onUpgrade(SQLiteDatabase db,int nOldVer,int nNewVer) {
    	if( nOldVer < 16 )
    	{
    		db.execSQL("DROP TABLE IF EXISTS " + SERVERS_TABLE_NAME + ";" );
    		db.execSQL("DROP TABLE IF EXISTS " + FOLDERS_TABLE_NAME + ";" );
    		db.execSQL("DROP TABLE IF EXISTS " + FILTERS_TABLE_NAME + ";" );
    		onCreate(db);
    		return;
    	}
    	if( nOldVer < 17 ) 
    	{
    		db.execSQL(SERVERS_TABLE_UPGRADE_V16_V17);
    		db.execSQL(FOLDERS_TABLE_UPGRADE_V16_V17);
    	}
    	if( nOldVer < 18 )
    	{
    		db.execSQL(FOLDERS_TABLE_UPGRADE_V17_V18);
    	}
    	if( nOldVer < 19 )
    	{
    		db.execSQL(UPLOADS_TABLE_CREATE);
    	}
    }
}