package uk.me.grambo.syncro;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;

import android.app.IntentService;
import android.content.Intent;
import android.net.Uri;

public class SyncroService extends IntentService {

	public SyncroService(String name) {
		super(name);
		// TODO Auto-generated constructor stub
	}

	@Override
	protected void onHandleIntent(Intent arg0) {
		if( arg0.getAction().equals("SYNCRO_SYNC") ) {
			Uri oURI = arg0.getData();
			String sHost = oURI.getHost();
			int nPort = oURI.getPort();
			RunSync(sHost,nPort);
		}
	}
	
	protected void RunSync(String insHost,int innPort) {
		try {
			Socket oSock = new Socket(insHost,innPort);
			InputStream oInput = oSock.getInputStream();
			OutputStream oOutput = oSock.getOutputStream();
			OutputStreamWriter oWriter = new OutputStreamWriter(oOutput);
			oWriter.write("Hello Syncro?");
			byte aHandshakeResponse[] = new byte[3];
			oInput.read(aHandshakeResponse, 0, 3);
			if( (aHandshakeResponse[0] == 100) && (aHandshakeResponse[1] == 200) && (aHandshakeResponse[2] == 50) ) {
				//We have a syncro server
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
