package uk.me.grambo.syncro;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.util.Vector;

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

public class SyncroService extends IntentService implements RemoteFileHandler{
	
	private static final int XML_REQUEST_FIRST_BYTE = 5;
	private static final int XML_RESPONSE_FIRST_BYTE = 6;
	private static final int FILE_REQUEST_FIRST_BYTE = 10;
	private static final int FILE_SECTION_REQUEST_FIRST_BYTE = 15;
	
	private static final int FILE_SEND_FIRST_BYTE = 11;
	private static final int FILE_SECTION_FIRST_BYTE = 16;
	private static final int FILE_LAST_SECTION_FIRST_BYTE = 20;
	
	private Vector<String> m_aFilesToDownload;

	public SyncroService() {
		super("SyncroService");
		// TODO Auto-generated constructor stub
		m_aFilesToDownload = new Vector<String>();
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
	        	SQLiteStatement oInsertStatement = oDB.compileStatement("INSERT INTO folders(ID,ServerID,Name,ServerPath) VALUES(?,1,?,?)");
				GetFolderList(oSock,oInsertStatement);
				GetFolderContents(oSock,0);
				GetFiles(oSock);
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
		int nSendSize = sRequest.length() + 1 + 4; 
		oOutput.writeInt( nSendSize );
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
	
	protected boolean GetFolderContents(Socket inoSock,int innFolderID) throws IOException {
		DataInputStream oInput = new DataInputStream( inoSock.getInputStream() );
		DataOutputStream oOutput = new DataOutputStream( inoSock.getOutputStream() );
		OutputStreamWriter oWriter = new OutputStreamWriter( inoSock.getOutputStream() );
		
		String sRequest = "GET_FOLDER_CONTENTS:";
		sRequest += (new Integer(innFolderID)).toString();
		oOutput.write( XML_REQUEST_FIRST_BYTE );
		//TODO: find out if the numbers added in the legnth below are correct
		int nSendSize = sRequest.length() + 1 + 4; 
		oOutput.writeInt( nSendSize );
		oOutput.flush();
		oWriter.write( sRequest );
		oWriter.flush();
		if( oInput.read() != XML_RESPONSE_FIRST_BYTE ) {
			return false;
		}
		int nSize = oInput.readInt();
		if( nSize > 5 ) {
			byte aData[] = new byte[nSize - 5];
			if( oInput.read(aData) != (nSize-5) ) {
				return false;
			}
			Log.i("Syncro",new String(aData) );
			FolderContentsXMLHandler oHandler = new FolderContentsXMLHandler();
			oHandler.AddFileHandler(this);
			ProcessXML(new ByteArrayInputStream(aData), oHandler );
			Log.i("Syncro","Files To Download:");
			for(int n=0;n < m_aFilesToDownload.size();n++) {
				Log.i("Syncro",m_aFilesToDownload.elementAt(n) );
			}
		}
		return true;
	}
	
	@Override
	public void HandleRemoteFile(String insFilename) {
		m_aFilesToDownload.add(insFilename);
	}
	
	protected boolean GetFiles(Socket inoSock) throws IOException {

		boolean fOK = false;		
		for(int nFile = 0;nFile < m_aFilesToDownload.size();nFile++) {
			fOK = GetFile( inoSock, m_aFilesToDownload.elementAt(nFile) );
			if( !fOK )
				return false;
		}
		return fOK;
	}
	
	protected boolean GetFile(Socket inoSock,String insFilename) throws IOException {
		
		boolean fOK = false;
		fOK = StartDownloadingFile(inoSock,insFilename);
		if( fOK ) {
			FileOutputStream oFile = new FileOutputStream( GetDestinationFilename( insFilename ) );
			fOK = ReceiveFile(inoSock,oFile);
			oFile.close();
		}
		return fOK;
	}
	
	protected String GetDestinationFilename(String insFilename) {
		return "/mnt/sdcard/1.txt";
	}

	private boolean StartDownloadingFile(Socket inoSock,String insFilename) throws IOException {
		byte aOutBuffer[] = new byte[ 1 + 4 + insFilename.length()];
		aOutBuffer[0] = FILE_REQUEST_FIRST_BYTE;
		
		DataOutputStream oOutput = new DataOutputStream( inoSock.getOutputStream() );
		OutputStreamWriter oWriter = new OutputStreamWriter( inoSock.getOutputStream() );
		
		oOutput.write( FILE_REQUEST_FIRST_BYTE );
		//TODO: again, check the numbers below
		oOutput.writeInt( insFilename.length() + 1 + 4 );
		oOutput.flush();
		oWriter.write( insFilename );
		oWriter.flush();
		
		return true;
	}
	
	private boolean ReceiveFile(Socket inoSock, FileOutputStream inoFile) throws IOException {
		DataInputStream oInput = new DataInputStream( inoSock.getInputStream() );
		DataOutputStream oOutput = new DataOutputStream( inoSock.getOutputStream() );
		
		//char aInBuffer[];
		byte nFirstByte;
		int nSize;
		int nFileOffset;
		byte nHashSize;
		nFirstByte = oInput.readByte();
		if( (nFirstByte != FILE_SEND_FIRST_BYTE) && (nFirstByte != FILE_LAST_SECTION_FIRST_BYTE) )
			return false;
			
		nSize = oInput.readInt();
		nFileOffset = oInput.readInt();
		nHashSize = oInput.readByte();
		
		int nLeftToRead = nSize - 10;	//nSize - sizeof(header)
		byte aData[] = new byte[nLeftToRead];
		oInput.read(aData);
		inoFile.write(aData);
		
		while( nFirstByte != FILE_LAST_SECTION_FIRST_BYTE ) {
			oOutput.write( FILE_SECTION_REQUEST_FIRST_BYTE );
			oOutput.writeInt( 1 + 4 );
			oOutput.flush();
			nFirstByte = oInput.readByte();
			if( (nFirstByte != FILE_SECTION_FIRST_BYTE) || (nFirstByte != FILE_LAST_SECTION_FIRST_BYTE) )
				return false;
				
			nSize = oInput.readInt();
			nFileOffset = oInput.readInt();
			nHashSize = oInput.readByte();
			nLeftToRead = nSize - 10;	//nSize - sizeof(header)
			
			if( aData.length < nLeftToRead )
				aData = new byte[nLeftToRead];
			oInput.read(aData,0,nLeftToRead);
			inoFile.write(aData,0,nLeftToRead);
		}
		
		return true;
	}
}
