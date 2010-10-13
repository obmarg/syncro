package uk.me.grambo.syncro;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteStatement;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.CursorAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

public class FolderGeneralSettings extends Activity {
	
	private int m_nServerID;
	private int m_nFolderID;

	public FolderGeneralSettings() {
		// TODO Auto-generated constructor stub
	}
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
		Intent parameterIntent = getIntent();
		m_nServerID = parameterIntent.getExtras().getInt("uk.me.grambo.syncro.server_id");
		m_nFolderID = parameterIntent.getExtras().getInt("uk.me.grambo.syncro.folder_id");
        
	    setContentView(R.layout.folder_general_settings);
	    
	    try {
	      	DBHelper oHelper = new DBHelper( this );
	       	SQLiteDatabase oDB = oHelper.getReadableDatabase();
	       	fillSettings(oDB);
		    oDB.close();
	    }catch(SQLException oException) {
	     	oException.printStackTrace();
	    }catch(Exception oException) {
	    	oException.printStackTrace();
	    }
    }
    
    public void onPause() {
    	super.onPause();
    	
	    try {
	      	DBHelper oHelper = new DBHelper( this );
	       	SQLiteDatabase oDB = oHelper.getWritableDatabase();
	       	commitSettings(oDB);
		    oDB.close();
	    }catch(SQLException oException) {
	     	oException.printStackTrace();
	    }catch(Exception oException) {
	    	oException.printStackTrace();
	    }
    }
    
	protected void fillSettings(SQLiteDatabase inoDB) throws Exception {
		String aArgs[] = { new Integer(m_nFolderID).toString() };
		Cursor oResults = inoDB.rawQuery("SELECT Name,LocalPath,SyncToPhone,SyncFromPhone From folders where id=?", aArgs);
		if( oResults.moveToFirst() ) {
	    	EditText oFolderNameEditText = (EditText)findViewById(R.id.folder_general_settings_name);
	    	oFolderNameEditText.setText( oResults.getString(0) );
	    	EditText oLocalPathEditText = (EditText)findViewById(R.id.folder_general_settings_dest_folder);
	    	oLocalPathEditText.setText( oResults.getString(1) );
	    	CheckBox oReceiveSyncBox = (CheckBox)findViewById(R.id.folder_general_settings_receive_sync);
	    	oReceiveSyncBox.setChecked( oResults.getLong(2) == 1 );
	    	CheckBox oSendSyncBox = (CheckBox)findViewById(R.id.folder_general_settings_send_sync);
	    	oSendSyncBox.setChecked( oResults.getLong(3) == 1 );
		} else {
			throw new Exception("No such folder");
		}
		oResults.close();
	}
	
	protected void commitSettings(SQLiteDatabase inoDB) throws Exception {
		SQLiteStatement oStatement = inoDB.compileStatement("UPDATE folders SET Name=?, LocalPath=?, SyncToPhone=?, SyncFromPhone =? WHERE ID=?" );
		EditText oFolderNameEditText = (EditText)findViewById(R.id.folder_general_settings_name);
		EditText oLocalPathEditText = (EditText)findViewById(R.id.folder_general_settings_dest_folder);
		CheckBox oReceiveSyncBox = (CheckBox)findViewById(R.id.folder_general_settings_receive_sync);
		CheckBox oSendSyncBox = (CheckBox)findViewById(R.id.folder_general_settings_send_sync);
		oStatement.bindString( 1, oFolderNameEditText.getText().toString() );
		oStatement.bindString( 2, oLocalPathEditText.getText().toString() );
		if ( oReceiveSyncBox.isChecked() ) {
			oStatement.bindLong( 3, 1 );
		} else {
			oStatement.bindLong( 3, 0 );
		}
		if ( oSendSyncBox.isChecked() ) {
			oStatement.bindLong( 4, 1 );
		} else {
			oStatement.bindLong( 4, 0 );
		}
		oStatement.bindLong(5, m_nFolderID);
		oStatement.execute();
		oStatement.close();
	}
   
}