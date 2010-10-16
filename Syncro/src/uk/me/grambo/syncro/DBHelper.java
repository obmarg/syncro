package uk.me.grambo.syncro;

import android.database.sqlite.*;
import android.content.*;

public class DBHelper extends SQLiteOpenHelper {

    private static final int DATABASE_VERSION = 16;
    private static final String DATABASE_NAME="SyncroDB";
    private static final String SERVERS_TABLE_NAME = "servers";
    private static final String SERVERS_TABLE_CREATE =
                "CREATE TABLE IF NOT EXISTS " + SERVERS_TABLE_NAME + " (" +
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
                "Name TEXT, " +
                "IP TEXT, " + 
                "Port INTEGER, " +
                "CONSTRAINT IP_NAME_UNIQUE UNIQUE (Name,IP) ON CONFLICT FAIL);";
    
    private static final String SERVERS_TABLE_UPGRADE_V2 =
    			"DROP TABLE IF EXISTS " + SERVERS_TABLE_NAME + " ;\n";
    
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
    			"CONSTRAINT PK_FOLDERS UNIQUE (IDOnServer,ServerID) ON CONFLICT IGNORE);";
    
    private static final String FILTERS_TABLE_NAME = "filters";
    private static final String FILTERS_TABLE_CREATE =
    			"CREATE TABLE IF NOT EXISTS " + FILTERS_TABLE_NAME + " (" +
    			"ID INTEGER PRIMARY KEY AUTOINCREMENT, " +
    			"FolderID INTEGER CONSTRAINT FILTERS_FOLDER_ID_FK REFERENCES " + FOLDERS_TABLE_NAME + "(ID) ON DELETE CASCADE ON UPDATE CASCADE, " + 
    			"FilterType INTEGER NOT NULL, " + 
    			"Name TEXT," +
    			"IncludeType INTEGER NOT NULL DEFAULT 0," +
    			"FilenameType INTEGER NOT NULL DEFAULT 0);";
    

    DBHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(SERVERS_TABLE_CREATE);
        db.execSQL(FOLDERS_TABLE_CREATE);
        db.execSQL(FILTERS_TABLE_CREATE);
    }
    
    @Override
    public void onUpgrade(SQLiteDatabase inDB,int nOldVer,int nNewVer) {
    	if( nOldVer <= 5 ) {
    		inDB.execSQL(SERVERS_TABLE_UPGRADE_V2);
    		inDB.execSQL(SERVERS_TABLE_CREATE);
    	}
    	if( nOldVer <= 12 ) {
    		inDB.execSQL("DROP TABLE IF EXISTS " + FOLDERS_TABLE_NAME + ";\n");
    		inDB.execSQL(FOLDERS_TABLE_CREATE);
    	}
    	if( nOldVer <= 13 ) {
    		inDB.execSQL("UPDATE folders SET SyncToPhone=1");
    		inDB.execSQL("UPDATE folders SET LocalPath='/mnt/sdcard/Syncro/'");
    	}
    	if( nOldVer < 16 ) {
    		inDB.execSQL("DROP TABLE IF EXISTS " + FILTERS_TABLE_NAME + ";\n");
    		inDB.execSQL(FILTERS_TABLE_CREATE);
    	}
    }
}