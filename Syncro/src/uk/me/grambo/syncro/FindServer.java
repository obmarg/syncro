package uk.me.grambo.syncro;

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
    	ListView oListView = (ListView)findViewById(R.id.findserverlist);
    	oListView.setAdapter(new ArrayAdapter<String>(this, R.layout.list_item, m_aServerNames ));
    }
}