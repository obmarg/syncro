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
import android.os.*;

public class FindServerBroadcaster extends AsyncTask<Void, Void, DatagramPacket> {
	private static final int DISCOVERY_PORT = 9995;
	private static final int PORT = 9996;
	private static final String DISCOVERY_STRING = "SyncroHELLO";
	private static final int SOCKET_TIMEOUT_MS = 30 * 1000;
	
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
	
	protected DatagramPacket doInBackground(Void... urls) {
		try {
			DatagramSocket oSocket;
			oSocket = new DatagramSocket(PORT);
			oSocket.setBroadcast(true);
			oSocket.setSoTimeout(SOCKET_TIMEOUT_MS);
	        DatagramPacket oOutPacket = new DatagramPacket(DISCOVERY_STRING.getBytes(), DISCOVERY_STRING.length(),
	            getBroadcastAddress(), DISCOVERY_PORT);
	        oSocket.send(oOutPacket);
	        
	        byte[] aRecvBuf = new byte[50];
	        DatagramPacket oInPacket = new DatagramPacket(aRecvBuf, aRecvBuf.length);
	        Log.d("Syncro", "Waiting for response");
	        oSocket.receive(oInPacket);
	        Log.d("Syncro", "Response recieved: ");
	        String s = new String(oInPacket.getData(), 0, oInPacket.getLength());
	        InetAddress oAddress = oInPacket.getAddress();
	        Log.d("Syncro", "Received response " + s);
	        Log.d("Syncro", "From: " + oAddress.toString() );
	        oSocket.close();
	        return oInPacket;
        }catch(IOException e) {
        	Log.e("Syncro", "IOException in FindServer::SendBroadcast:" );
        	Log.e("Syncro", e.toString() );
        	return null;
        }
	}
	
	/*protected void onPostExecute(DatagramPacket inoResult) {
		if( inoResult == null )
			return;
		Log.d("Syncro", "In post executte");
		String s = new String(inoResult.getData(), 0, inoResult.getLength());
		String[] asSections = s.split(": "); 
		m_oParent.AddServer( asSections[1],inoResult.getAddress() );
	}*/
        
}
