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

import android.app.Activity;
import android.os.Bundle;
import android.widget.*;
import android.database.*;
import android.database.sqlite.*;
import android.content.*;
import android.view.*;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.util.Log;
import java.net.*;
import java.io.*;

import uk.me.grambo.syncro.DBHelper;
import uk.me.grambo.syncro.R;
import uk.me.grambo.syncro.R.id;
import uk.me.grambo.syncro.R.layout;
import uk.me.grambo.syncro.comms.FindServerBroadcaster;
import android.os.AsyncTask;

public class FindServer extends Activity
{	
	private class ServerEntry {
		public String Name;
		public InetAddress Address;
	}
	private ServerEntry[] m_aServers;
	private String[] m_aServerNames;
	
	FindServerBroadcaster m_oBroadcaster;
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.findserver);
        
        Button oRefresh = (Button)findViewById(R.id.refresh);
        oRefresh.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
            		FindServer.this.SendBroadcast();
                  }
        });
        
        ListView oListView = (ListView)findViewById(R.id.findserverlist);
        oListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
        	public void onItemClick(AdapterView oParent,View oView,int innPosition,long innID) {
        	    try {
        	       	DBHelper oHelper = new DBHelper( oParent.getContext() );
        	      	SQLiteDatabase oDB = oHelper.getReadableDatabase();
        	      	//TODO: update this stuff to supply an actual port
        	      	String aArgs[] = new String[3];
        	      	aArgs[0] = FindServer.this.m_aServers[innPosition].Name;
        	      	//TODO: possibly replace the below with gethostaddress?
        	      	aArgs[1] = FindServer.this.m_aServers[innPosition].Address.getHostName();
        	      	aArgs[2] = Integer.toString( 9998 );
        	     	oDB.execSQL( "INSERT INTO servers(Name,Ip,Port) VALUES(?,?,?);", aArgs );
        		    oDB.close();
        		    
        		    Context context = getApplicationContext();
        		    CharSequence text = "Added server: " + aArgs[0];
        		    int duration = Toast.LENGTH_SHORT;
        		    Toast toast = Toast.makeText(context, text, duration);
        		    toast.show();
        		    //TODO: call a function to remove this server from the list
        		    //FindServer.this.RemoveServer(innPosition);
        	    }catch(SQLException oException) {
        	     	oException.printStackTrace();
        	    }
        	};
		});
        
        SendBroadcast();
    }
    
    public void SendBroadcast() {
    	if( (m_oBroadcaster == null) || (m_oBroadcaster.getStatus() != AsyncTask.Status.RUNNING) ) {
    		m_oBroadcaster = new FindServerBroadcaster( (WifiManager)getSystemService(Context.WIFI_SERVICE), this );
    		Toast.makeText(getApplicationContext(), "Sent Broadcast Packet", Toast.LENGTH_SHORT).show();
    		m_oBroadcaster.execute();
    	}
    }
    
    public void AddServer(String insName,InetAddress inoAddress) {
    	//TODO: check that entries aren't in the database already.
    	Log.d("Syncro", "Adding server: " + insName + " : " + inoAddress.toString() );
    	ServerEntry[] aNewArray;
    	int nInsertIndex = 0;
    	if( m_aServers != null ) {
    		for(int n=0;n < m_aServers.length; n++ ) {
    			if( ( m_aServers[n].Name.equals(insName) ) && ( m_aServers[n].Address.equals(inoAddress) ) ) {
    				//Server already exists
    				return;
    			}
    		}
    		nInsertIndex = m_aServers.length;
    		aNewArray = new ServerEntry[ m_aServers.length + 1 ];
    		System.arraycopy(m_aServers, 0, aNewArray, 0, m_aServers.length );
    	} else {
    		aNewArray = new ServerEntry[ 1 ];
    	}
    	aNewArray[ nInsertIndex ] = new ServerEntry();
    	aNewArray[ nInsertIndex ].Name = insName;
		aNewArray[ nInsertIndex ].Address = inoAddress;
    	m_aServers = aNewArray;
    	m_aServerNames = new String[ m_aServers.length ];
    	for(int n=0; n< m_aServers.length; n++ ) {
    		m_aServerNames[n] = m_aServers[n].Name;
    	}
    	BuildServerList();
    }
    
    //TODO: Implement some server removing function
    
    public void BuildServerList()
    {
    	ListView oListView = (ListView)findViewById(R.id.findserverlist);
    	oListView.setAdapter(new ArrayAdapter<String>(this, R.layout.list_item, m_aServerNames ));
    }
}