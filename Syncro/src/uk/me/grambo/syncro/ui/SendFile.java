package uk.me.grambo.syncro.ui;

import uk.me.grambo.syncro.DBHelper;
import uk.me.grambo.syncro.R;
import uk.me.grambo.syncro.SyncroService;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
import android.database.sqlite.SQLiteStatement;
import android.net.Uri;
import android.os.Bundle;
import android.widget.Toast;

public class SendFile extends Activity implements DialogInterface.OnClickListener {

	private class DialogEntryInfo {
		public int ServerID;
		public int FolderID;
	}
	private DialogEntryInfo[] m_entries;
	private AlertDialog.Builder m_dialog;
	
	private String m_filePath;
	
	private SQLiteDatabase m_db;
	private SQLiteStatement m_fileInsert;
	
	public SendFile() {
		m_db = null;
	}
	
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        Intent startingIntent = getIntent();
        /*if( startingIntent.getAction().equals( Intent.ACTION_SEND ) )
        {
        	return;
        }*/
        
        Uri uri;
        try
        {
	        Bundle bund = startingIntent.getExtras();
	        uri = (Uri)bund.get( Intent.EXTRA_STREAM );
        }
        catch( Exception e )
        {
        	e.printStackTrace();
        	ShowErrorDialog( "Syncro can not send this data" );
        	return;
        }
        if( uri == null || !uri.getScheme().equals( "file" ) )
        {
        	ShowErrorDialog( "Syncro can not send this data" );
        	return;
        }
        m_filePath = uri.getPath();
        
        Cursor results = null;
        CharSequence options[] = null;
        try
        {
	        //TODO: Check the intent?
	        DBHelper helper = new DBHelper( this );
	        m_db = helper.getWritableDatabase();
	        results = m_db.rawQuery(
	        		"SELECT f.IDOnServer,f.Name,s.ID,s.Name FROM " +
	        		"folders AS f LEFT JOIN servers AS s " +
	        		"ON f.ServerID=s.ID WHERE f.SyncFromPhone=1 " +
	        		"AND f.Deleted=0",
	        		new String[0]
	        		);
	        if( !results.moveToFirst() )
	        {
	        	ShowErrorDialog( "Could not find a folder set to upload to PC" );
	        	return;
	        }
	        
	        options = 
	        	new CharSequence[ results.getCount() ];
	        m_entries = 
	        	new DialogEntryInfo[ results.getCount() ];
	        
	        int optNum = 0;
	        do
	        {
	        	m_entries[optNum] = new DialogEntryInfo();
	        	m_entries[optNum].FolderID = results.getInt(0);
	        	m_entries[optNum].ServerID = results.getInt(2);
	        	options[optNum] = 
	        		results.getString(1) + " on " + results.getString(3);
	        	++optNum;
	        }while( results.moveToNext() );
	        
	        m_fileInsert = m_db.compileStatement(
	        		"INSERT INTO uploads( FolderID, Filename ) " +
	        		"VALUES( ?, ? )"
	        		);
        }
        catch( SQLException e )
        {
        	e.printStackTrace();
        	ShowErrorDialog( "SQL Error!" );
        	return;
        }
        finally
        {
        	if( results != null )
        		results.close();
        }
        
        m_dialog = new AlertDialog.Builder(this);
        m_dialog.setTitle( "Where do you want to send your file?" );
        m_dialog.setItems( options, this );
        AlertDialog alert = m_dialog.create();
        alert.show();
    }
    
    public void onResume()
    {
    	super.onResume();
    	if( m_db == null )
    	{
    		try 
    		{
    			DBHelper helper = new DBHelper( this );
    			m_db = helper.getWritableDatabase();
    		}
    		catch( SQLiteException e )
    		{
    			e.printStackTrace();
    		}
    	}
    }
    
    public void onPause()
    {
    	super.onPause();
    	m_db.close();
    	m_db = null;
    }

	private void ShowErrorDialog(String error) 
	{
		AlertDialog.Builder builder =
			new AlertDialog.Builder(this);
		builder.setTitle( "Error" );
		builder.setMessage( error );
		builder.setNeutralButton(
			"Ok", 
			new DialogInterface.OnClickListener() 
			{
				@Override
				public void onClick(DialogInterface dialog, int which) 
				{
					SendFile.this.finish();
				}
			}
			);
		
		AlertDialog alert = builder.create();
		alert.show();
	}

	@Override
	public void onClick(DialogInterface dialog, int which) 
	{
		if( m_fileInsert == null )
		{
			ShowErrorDialog("File insert statement not created"	);
			return;
		}
		m_fileInsert.bindLong( 1, m_entries[which].FolderID );
		m_fileInsert.bindString( 2 , m_filePath );
		m_fileInsert.execute();
		
		Intent i = new Intent( this, SyncroService.class );
		i.setAction("uk.me.grambo.syncro.SYNCRO_SYNC");
		i.setData( Uri.parse( 
				"syncroid://" + m_entries[which].ServerID 
				) );
		startService( i );
		finish();
	}
}
