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

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Resources;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TabHost;
import android.widget.TextView;
import uk.me.grambo.syncro.FolderFilterSettings;

public class ServerConfig extends Activity  {
	
	private class FolderListUpdateReceiver extends BroadcastReceiver {
		private ServerConfig m_parent;
		
		public FolderListUpdateReceiver(ServerConfig parent)
		{
			m_parent = parent;
		}
		
		public void onReceive(Context context, Intent intent) {
			//TODO: at some point, check the contents of the intent.
			//		for now, just update the folder list.
			m_parent.setupUI();
		}
	}
	
	private FolderListUpdateReceiver m_broadcastReceiver;	
	private int m_nServerID;
	private String m_aServerDetails[];
	
	private String m_aFolders[];
	private int m_aFolderIDs[];
	


	public ServerConfig() {
		m_broadcastReceiver = new FolderListUpdateReceiver(this);
	}
	
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.server_config);
		
		Intent parameterIntent = getIntent();
		m_nServerID = parameterIntent.getExtras().getInt("uk.me.grambo.syncro.server_id");
		
		ListView oFolderList = (ListView)findViewById(R.id.server_config_folder_list);
		
		oFolderList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
        	public void onItemClick(AdapterView oParent,View oView,int innPosition,long innID) {
        		Intent i = new Intent(ServerConfig.this,FolderConfig.class);
        		i.putExtra( "uk.me.grambo.syncro.server_id", m_nServerID );
        		i.putExtra( "uk.me.grambo.syncro.folder_id", m_aFolderIDs[innPosition] );
        		ServerConfig.this.startActivity(i);
        	};
		});
		
		Button oSyncNowButton = (Button)findViewById(R.id.server_config_sync_now);
		
		oSyncNowButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
        		Intent i = new Intent( ServerConfig.this, SyncroService.class );
        		i.setAction("uk.me.grambo.syncro.SYNCRO_SYNC");
        		i.setData( Uri.parse( "syncroid://" + m_nServerID ) );
        		ServerConfig.this.startService( i );
            }
        });
		
		Button oDeleteServerButton = (Button)findViewById(R.id.server_config_delete_server);
		oDeleteServerButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
        		ServerConfig.this.deleteServer();
        		ServerConfig.this.finish();
            }
        });
		
		setupUI();
	}
	
	public void onResume() {
		super.onResume();
		IntentFilter filter = new IntentFilter();
		filter.addAction("uk.me.grambo.syncro.ui.FOLDER_LIST_UPDATE");
		registerReceiver( m_broadcastReceiver, filter );
		//setupUI();
		//TODO: setupUI etc.
		//TODO: find out what method this stuff should be done in.  needs to be done fast + only once ideally
	}
	
	public void onPause() {
		super.onPause();
		unregisterReceiver(m_broadcastReceiver);
	}
	
	protected void setupUI() {
		ListView oDetailsList = (ListView)findViewById(R.id.server_config_details_list);
		ListView oFolderList = (ListView)findViewById(R.id.server_config_folder_list);
		try {
	      	DBHelper oHelper = new DBHelper( this );
	       	SQLiteDatabase oDB = oHelper.getReadableDatabase();
	       	fillDetailsList(oDB,oDetailsList);
	       	fillFolderList(oDB,oFolderList);
		    oDB.close();
	    }catch(SQLException oException) {
	     	oException.printStackTrace();
	    }catch(Exception oException) {
	    	oException.printStackTrace();
	    }
	}
	
	protected void fillDetailsList(SQLiteDatabase inoDB,ListView inoList) throws Exception {
		String aArgs[] = { new Integer(m_nServerID).toString() };
		Cursor oResults = inoDB.rawQuery("SELECT Name,IP,Port From servers where id=?", aArgs);
		if( oResults.moveToFirst() ) {
			m_aServerDetails = new String[3];
			m_aServerDetails[0] = "Name: " + oResults.getString(0);
			m_aServerDetails[1] = "IP: " + oResults.getString(1);
			m_aServerDetails[2] = "Port: " + oResults.getString(2);
			
			inoList.setAdapter(new ArrayAdapter<String>(this, R.layout.list_item, m_aServerDetails ));
		} else {
			throw new Exception("No such server");
		}
		
		
	}
	
    protected void fillFolderList(SQLiteDatabase inoDB,ListView inoList) {
    	String[] aArgs = { (new Integer(m_nServerID).toString()) };
    	Cursor oResults = inoDB.rawQuery("SELECT Name,ID From folders where ServerID=?", aArgs);
        int nResults = oResults.getCount();
        if( nResults > 0 ) {
        	m_aFolders = new String[nResults];
        	m_aFolderIDs = new int[nResults];
        	oResults.moveToFirst();
        	int n = 0;
        	do {
        		m_aFolders[n] = oResults.getString(0);
        		m_aFolderIDs[n] = oResults.getInt(1);
        		n++;
        	} while( oResults.moveToNext() );
        	
        }
        if( nResults != 0 ) {
        	inoList.setAdapter(new ArrayAdapter<String>(this, R.layout.list_item, m_aFolders ));
        }
        oResults.close();
    }
    
    protected void deleteServer() {
		try {
	      	DBHelper oHelper = new DBHelper( this );
	       	SQLiteDatabase oDB = oHelper.getWritableDatabase();
	       	Integer aArgs[] = { m_nServerID };
	       	oDB.execSQL("DELETE FROM servers WHERE ID=?", aArgs );
		    oDB.close();
	    }catch(SQLException oException) {
	     	oException.printStackTrace();
	    }catch(Exception oException) {
	    	oException.printStackTrace();
	    }
    }

	
}
