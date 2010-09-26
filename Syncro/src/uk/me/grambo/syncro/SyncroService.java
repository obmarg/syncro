package uk.me.grambo.syncro;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.helpers.DefaultHandler;

import android.app.IntentService;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteStatement;
import android.net.Uri;
import android.util.Log;

public class SyncroService extends IntentService {
	
	private static final int XML_REQUEST_FIRST_BYTE = 5;
	private static final int XML_RESPONSE_FIRST_BYTE = 6;

	public SyncroService() {
		super("SyncroService");
		// TODO Auto-generated constructor stub
	}

	@Override
	protected void onHandleIntent(Intent arg0) {
		if( arg0.getAction().equals("uk.me.grambo.syncro.SYNCRO_SYNC") ) {
			Uri oURI = arg0.getData();
			if( oURI != null ) {
				String sHost = oURI.getHost();
				int nPort = oURI.getPort();
				RunSync(sHost,nPort);
			}
		}
	}
	
	protected void ProcessXML(InputStream inoData,DefaultHandler inoHandler) {
		SAXParserFactory oFactory = SAXParserFactory.newInstance();
		try {
			SAXParser oParser = oFactory.newSAXParser();
			oParser.parse(inoData,inoHandler);
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}
	
	protected void RunSync(String insHost,int innPort) {
		try {
			Socket oSock = new Socket(insHost,innPort);
			if( DoHandshake( oSock ) ) {
				DBHelper oHelper = new DBHelper( this );
	        	SQLiteDatabase oDB = oHelper.getReadableDatabase();
	        	SQLiteStatement oInsertStatement = oDB.compileStatement("INSERT INTO folders(ServerID,Name,ServerPath) VALUES(1,?,?)");
				GetFolderList(oSock,oInsertStatement);
				oDB.close();
				oDB = null;
			}
			oSock.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	protected boolean DoHandshake(Socket inoSock) throws IOException {
		InputStream oInput = inoSock.getInputStream();
		OutputStream oOutput = inoSock.getOutputStream();
		OutputStreamWriter oWriter = new OutputStreamWriter(oOutput);
		oWriter.write("Hello Syncro?");
		oWriter.flush();
		byte aHandshakeResponse[] = new byte[3];
		oInput.read(aHandshakeResponse, 0, 3);
		if( (aHandshakeResponse[0] == 100) && (aHandshakeResponse[1] == 118) && (aHandshakeResponse[2] == 50) ) {
			return true;
		}
		return false;
	}
	
	protected boolean GetFolderList(Socket inoSock,SQLiteStatement inoInsertStatement) throws IOException {
		DataInputStream oInput = new DataInputStream( inoSock.getInputStream() );
		DataOutputStream oOutput = new DataOutputStream( inoSock.getOutputStream() );
		OutputStreamWriter oWriter = new OutputStreamWriter( inoSock.getOutputStream() );
		
		String sRequest = "GET_FOLDER_LIST";
		oOutput.write( XML_REQUEST_FIRST_BYTE );
		oOutput.write( sRequest.length() );
		oOutput.flush();
		oWriter.write( sRequest );
		oWriter.flush();
		int nSize;
		if( oInput.read() != XML_RESPONSE_FIRST_BYTE ) {
			return false;
		}
		nSize = oInput.readInt();
		if( nSize > 5 ) {
			byte aData[] = new byte[nSize - 5];
			if( oInput.read(aData) != (nSize-5) ) {
				return false;
			}
			Log.i("Syncro",new String(aData) );
			ProcessXML(new ByteArrayInputStream(aData), new FolderListXMLHandler(inoInsertStatement));
		}
		return true;
	}

}
