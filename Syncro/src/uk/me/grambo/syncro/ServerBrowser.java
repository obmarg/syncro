package uk.me.grambo.syncro;

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
    String[] m_aServerAddresses;
    int[] m_aServerPorts;
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
        
        ListView oListView = (ListView)findViewById(R.id.serverlist);
        try {
        	DBHelper oHelper = new DBHelper( this );
        	SQLiteDatabase oDB = oHelper.getReadableDatabase();
        	fillServerList(oDB,oListView);
	        oDB.close();
        }catch(SQLException oException) {
        	oException.printStackTrace();
        }
        oListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
        	public void onItemClick(AdapterView oParent,View oView,int innPosition,long innID) {
        		Intent i = new Intent( ServerBrowser.this, SyncroService.class );
        		i.setAction("uk.me.grambo.syncro.SYNCRO_SYNC");
        		//TODO: Make this actually pass in an actual address from the db etc.
        		//i.setData(Uri.parse("syncro://10.0.2.2:9998"));
        		i.setData(Uri.parse("syncro://192.168.5.5:9998"));
        		ServerBrowser.this.startService( i );
        	};
		});
        
        
        Button oManualAdd = (Button)findViewById(R.id.addserverbutton);
        oManualAdd.setOnClickListener(new View.OnClickListener() {
        	public void onClick(View inView) {
        		ServerBrowser.this.showDialog(DIALOG_ADDSERVER);
        	}
        });
    }
    
    protected void fillServerList(SQLiteDatabase inoDB,ListView inoListview) {
    	Cursor oResults = inoDB.rawQuery("SELECT Name,IP,Port From servers", null);
        int nResults = oResults.getCount();
        if( nResults > 0 ) {
        	m_aServers = new String[nResults];
        	m_aServerAddresses = new String[nResults];
        	m_aServerPorts = new int[nResults];
        	oResults.moveToFirst();
        	int n = 0;
        	do {
        		m_aServers[n] = oResults.getString(0);
        		m_aServerAddresses[n] = oResults.getString(1);
        		m_aServerPorts[n] = oResults.getInt(2);
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
