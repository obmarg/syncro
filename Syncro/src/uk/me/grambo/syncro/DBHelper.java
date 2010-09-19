package uk.me.grambo.syncro;

import android.database.sqlite.*;
import android.content.*;

public class DBHelper extends SQLiteOpenHelper {

    private static final int DATABASE_VERSION = 2;
    private static final String DATABASE_NAME="SyncroDB";
    private static final String SERVERS_TABLE_NAME = "servers";
    private static final String SERVERS_TABLE_CREATE =
                "CREATE TABLE " + SERVERS_TABLE_NAME + " (" +
                "Name TEXT, " +
                "IP TEXT, " + 
                "Port INTEGER);";
    
    private static final String SERVERS_TABLE_UPGRADE_V1 = 
    			"ALTER TABLE " + SERVERS_TABLE_NAME +
    			"ADD COLUMN Port INTEGER";

    DBHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(SERVERS_TABLE_CREATE);
    }
    
    @Override
    public void onUpgrade(SQLiteDatabase inDB,int nOldVer,int nNewVer) {
    	if( nOldVer == 1 ) {
    		inDB.execSQL(SERVERS_TABLE_UPGRADE_V1);
    	}
    }
}