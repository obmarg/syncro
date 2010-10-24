package uk.me.grambo.syncro;

import android.app.Activity;
import android.os.Bundle;
import android.widget.*;
import android.database.*;
import android.database.sqlite.*;
import android.content.*;
import android.view.*;
import android.net.DhcpInfo;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.util.Log;
import java.net.*;
import java.util.ArrayList;
import java.io.*;
import android.os.*;

public class FindServerBroadcaster extends AsyncTask< Void, Void, ArrayList<FindServerBroadcaster.ServerInfo> > {
	private static final int DISCOVERY_PORT = 9995;
	private static final int PORT = 9996;
	private static final String DISCOVERY_STRING = "SyncroHELLO";
	private static final int SOCKET_TIMEOUT_MS = 500;
	private static final int SEARCH_TIME = 2 * 1000;
	
	public class ServerInfo {
		public String Name;
		public InetAddress Address;
		public int Port;
	}
	
	private WifiManager m_oWifiMan;
	private FindServer m_oParent;
	
	public FindServerBroadcaster(WifiManager inoWifi,FindServer inoParent) {
		m_oWifiMan = inoWifi;
		m_oParent = inoParent;
	}
    
	InetAddress getBroadcastAddress() throws IOException {
        DhcpInfo dhcp = m_oWifiMan.getDhcpInfo();
        // handle null somehow

        int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
        byte[] quads = new byte[4];
        for (int k = 0; k < 4; k++)
          quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
        Log.i("Syncro","Broadcasting on address: " + InetAddress.getByAddress(quads).toString() );
        return InetAddress.getByAddress(quads);
    }
	
	protected ArrayList<ServerInfo> doInBackground(Void... urls) {
		//TODO: Add some sort of "in progress" stuff to this
        ArrayList<ServerInfo> aRV = new ArrayList<ServerInfo>();
		DatagramSocket oSocket = null;
		try {
			oSocket = new DatagramSocket(PORT);
			oSocket.setBroadcast(true);
			oSocket.setSoTimeout(SOCKET_TIMEOUT_MS);
	        DatagramPacket oOutPacket = new DatagramPacket(DISCOVERY_STRING.getBytes(), DISCOVERY_STRING.length(),
	            getBroadcastAddress(), DISCOVERY_PORT);
	        oSocket.send(oOutPacket);
	        
	        long nEndTime = SystemClock.uptimeMillis() + SEARCH_TIME;
	        while( !oSocket.isClosed() && (SystemClock.uptimeMillis() < nEndTime) ) {
		        byte[] aRecvBuf = new byte[50];
		        DatagramPacket oInPacket = new DatagramPacket(aRecvBuf, aRecvBuf.length);
		        Log.d("Syncro", "Waiting for response");
		        try {
		        	oSocket.receive(oInPacket);
		        }catch(SocketTimeoutException e) {
		        	//Socket timed out, doesn't really matter
		        	continue;
		        }
		        String s = new String(oInPacket.getData(), 0, oInPacket.getLength());
		        
		        Log.d("Syncro", "Received response: " + s);
		        Log.d("Syncro", "From: " + oInPacket.getAddress().getHostName() );
		        
				String[] asSections = s.split(": ");
				if( asSections.length == 2 ) {
					ServerInfo oInfo = new ServerInfo();
					oInfo.Address = oInPacket.getAddress();
					oInfo.Name = asSections[1];
					//TODO: Update this to be read from the data
					//		after updating to use protocol buffers probably
					oInfo.Port = 9998;
					
					aRV.add( oInfo );
				} else {
					Log.d("Syncro","Invalid packet received in FindServerBroadcaster");
				}
	        }
        }catch(IOException e) {
        	Log.e("Syncro", "IOException in FindServer::SendBroadcast:" );
        	Log.e("Syncro", e.toString() );
        	return null;
        }
        finally {
        	if( (oSocket != null) && !oSocket.isClosed() ) {
        		oSocket.close();
        	}
        }
        return aRV;
	}
	
	protected void onPostExecute(ArrayList<ServerInfo> inoResult) {
		if( inoResult == null )
			return;
		Log.d("Syncro", "In post executte");
		
		for( int n=0; n < inoResult.size(); n++ ) {
			//TODO: pass in port as well
			m_oParent.AddServer( inoResult.get(n).Name, inoResult.get(n).Address );
		}
		
	}
        
}
