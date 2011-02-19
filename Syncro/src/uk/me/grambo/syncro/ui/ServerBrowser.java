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

package uk.me.grambo.syncro.ui;

import uk.me.grambo.syncro.DBHelper;
import uk.me.grambo.syncro.R;
import uk.me.grambo.syncro.R.id;
import uk.me.grambo.syncro.R.layout;
import android.app.Activity;
import android.net.Uri;
import android.os.Bundle;
import android.widget.*;
import android.database.*;
import android.database.sqlite.*;
import android.content.*;
import android.view.*;
import android.app.Dialog;
import android.util.Log;


public class ServerBrowser extends Activity
{	
	private static final int DIALOG_ADDSERVER = 0;
	
	
	private Dialog m_oAddServerDialog;
	
    String[] m_aServers;
    int[] m_aServerIDs;
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        Button oFindServer = (Button)findViewById(R.id.findserver);
        oFindServer.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
            	Intent i = new Intent(ServerBrowser.this,FindServer.class);
            	ServerBrowser.this.startActivity(i);
            	//Toast.makeText(getApplicationContext(), "Hello!",
                //        Toast.LENGTH_SHORT).show();
                  }
        });
        
        ListView oServerList = (ListView)findViewById(R.id.serverlist);

        oServerList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
        	public void onItemClick(AdapterView oParent,View oView,int innPosition,long innID) {
        		Intent i = new Intent(ServerBrowser.this,ServerConfig.class);
        		i.putExtra( "uk.me.grambo.syncro.server_id", m_aServerIDs[innPosition] );
        		ServerBrowser.this.startActivity(i);
        	};
		});
        
        
        Button oManualAdd = (Button)findViewById(R.id.addserverbutton);
        oManualAdd.setOnClickListener(new View.OnClickListener() {
        	public void onClick(View inView) {
        		ServerBrowser.this.showDialog(DIALOG_ADDSERVER);
        	}
        });
        
        fillData();
    }
    
    public void onRestart() {
    	super.onRestart();
    	fillData();
    }
    
    protected void fillData() {
    	ListView oServerList = (ListView)findViewById(R.id.serverlist);
        try {
        	DBHelper oHelper = new DBHelper( this );
        	SQLiteDatabase oDB = oHelper.getReadableDatabase();
        	fillServerList(oDB,oServerList);
	        oDB.close();
        }catch(SQLException oException) {
        	oException.printStackTrace();
        }
    }
    
    protected void fillServerList(SQLiteDatabase inoDB,ListView inoListview) {
    	Cursor oResults = inoDB.rawQuery("SELECT Name,ID From servers", null);
        int nResults = oResults.getCount();
        if( nResults > 0 ) {
        	m_aServers = new String[nResults];
        	m_aServerIDs = new int[nResults];
        	oResults.moveToFirst();
        	int n = 0;
        	do {
        		m_aServers[n] = oResults.getString(0);
        		m_aServerIDs[n] = oResults.getInt(1);
        		n++;
        	} while( oResults.moveToNext() );
        	
        }
        if( nResults != 0 ) {
        	inoListview.setAdapter(new ArrayAdapter<String>(this, R.layout.list_item, m_aServers ));
        }
        oResults.close();
    }
    
    protected Dialog onCreateDialog(int innID) {
    	Log.d("Syncro","onCreateDialog entry");
    	Dialog oOutDialog = null;
    	switch( innID ) {
    		case DIALOG_ADDSERVER:
    			if( m_oAddServerDialog != null )
    				Log.e("Syncro", "Added Addserverdialog more than once!");
    			m_oAddServerDialog = new Dialog( this ); 
    			oOutDialog = m_oAddServerDialog;
    			Log.d("Syncro","onCreateDialog created dialog");
    			oOutDialog.setContentView(R.layout.dialog_addserver);
    			Log.d("Syncro","onCreateDialog assigned content");
    			oOutDialog.setTitle("Enter Server Address...");
    			
    			Button oButton = (Button)oOutDialog.findViewById(R.id.dialog_addserver_ok);
    			oButton.setOnClickListener( new View.OnClickListener() {
    				public void onClick(View v) {
    					ServerBrowser.this.AddServer();
    				};
    			});
    			oButton = (Button)oOutDialog.findViewById(R.id.dialog_addserver_cancel);
    			oButton.setOnClickListener( new View.OnClickListener() {
    				public void onClick(View v) {
    					ServerBrowser.this.CloseAddServerDialog();
    				};
    			});
    			
    			break;
    		default:
    			break;
    	}
    	Log.d("Syncro","onCreateDialog exit");
    	return oOutDialog;
    }
    
    public void AddServer() {
    	EditText oServerNameEditText = (EditText)m_oAddServerDialog.findViewById(R.id.dialog_addserver_servername);
    	EditText oAddressEditText = (EditText)m_oAddServerDialog.findViewById(R.id.dialog_addserver_address);
    	EditText oPortEditText = (EditText)m_oAddServerDialog.findViewById(R.id.dialog_addserver_port);
    	
    	
    	String sServerName = oServerNameEditText.getText().toString();
    	String sAddress = oAddressEditText.getText().toString();
    	int nPort = Integer.parseInt(oPortEditText.getText().toString());
    	
    	//TODO: has to be a better/more efficient way of dealing with databases than this
    	DBHelper oHelper = new DBHelper( this );
    	SQLiteDatabase oDB = oHelper.getReadableDatabase();
        
    	SQLiteStatement oAddServerStatement = oDB.compileStatement("INSERT INTO servers(Name,IP,Port) VALUES(?,?,?)");
    	
        oAddServerStatement.bindString(1,sServerName);
        oAddServerStatement.bindString(2,sAddress);
        oAddServerStatement.bindLong(3,nPort);
        int nResult = (int)oAddServerStatement.executeInsert();
        if( nResult == -1 )
        	Log.e("Syncro", "execute failed");
        else
        	Log.d("Syncro", "execute success");
        ListView oListView = (ListView)findViewById(R.id.serverlist);
        fillServerList(oDB,oListView);
        oDB.close();
        m_oAddServerDialog.dismiss();
    }
    
    public void CloseAddServerDialog() {
    	m_oAddServerDialog.cancel();
    }
}
