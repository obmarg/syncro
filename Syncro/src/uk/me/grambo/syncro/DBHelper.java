package uk.me.grambo.syncro;

import android.database.sqlite.*;
import android.content.*;

public class DBHelper extends SQLiteOpenHelper {

    private static final int DATABASE_VERSION = 6;
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

    DBHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(SERVERS_TABLE_CREATE);
    }
    
    @Override
    public void onUpgrade(SQLiteDatabase inDB,int nOldVer,int nNewVer) {
    	if( nOldVer <= 5 ) {
    		inDB.execSQL(SERVERS_TABLE_UPGRADE_V2);
    		inDB.execSQL(SERVERS_TABLE_CREATE);
    	}
    }
}