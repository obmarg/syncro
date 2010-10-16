package uk.me.grambo.syncro;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
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

import uk.me.grambo.syncro.pb.Binarydata;

import android.app.IntentService;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteStatement;
import android.net.Uri;
import android.util.Log;

public class SyncroService extends IntentService implements RemoteFileHandler{
	
	private static final int XML_REQUEST_FIRST_BYTE = 5;
	private static final int XML_RESPONSE_FIRST_BYTE = 6;
	
	private Vector<RemoteFileHandler.RemoteFileData> m_aFilesToDownload;
	private PBSocketInterface m_oPBInterface;
	
	String m_sCurrentLocalPath;

	public SyncroService() {
		super("SyncroService");
		// TODO Auto-generated constructor stub
		m_aFilesToDownload = new Vector<RemoteFileHandler.RemoteFileData>();
		m_oPBInterface = new PBSocketInterface();
	}

	@Override
	protected void onHandleIntent(Intent arg0) {
		if( arg0.getAction().equals("uk.me.grambo.syncro.SYNCRO_SYNC") ) {
			Uri oURI = arg0.getData();
			if( oURI != null ) {
				String oScheme = oURI.getScheme();
				if( oScheme.equals("syncro") ) { 
					String sHost = oURI.getHost();
					int nPort = oURI.getPort();
					//RunSync(0,sHost,nPort);
				} else if( oScheme.equals("syncroid") ) {
			    	DBHelper oHelper = new DBHelper( this );
			    	SQLiteDatabase oDB = oHelper.getReadableDatabase();
			    	String aSQLArgs[] = new String[1];
			    	aSQLArgs[0] = oURI.getHost();
			    	Cursor oResults = oDB.rawQuery("SELECT ID,IP,Port FROM servers WHERE ID=?", aSQLArgs);
			    	if( oResults.moveToFirst() ) {
			    		int nID,nPort;
			    		String sAddress;
			    		nID = oResults.getInt(0);
			    		sAddress = oResults.getString(1);
			    		nPort = oResults.getInt(2);
			    		oResults.close();
			    		oDB.close();
			    		RunSync( nID, sAddress, nPort );
			    	} else {
			    		oDB.close();
			    	}
				}
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
	
	protected void RunSync(int innServerID,String insHost,int innPort) {
		//TODO: probably want to move innServerID into a member variable or something
		NotificationManager oNM = (NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);
		
		int icon = R.drawable.stat_sys_warning;
		CharSequence tickerText = "Syncro running";
		long when = System.currentTimeMillis();

		Notification oNotification = new Notification(icon, tickerText, when);
		
		Context context = getApplicationContext();
		CharSequence contentTitle = "Syncro Is Running!";
		CharSequence contentText = "Syncro is running!";
		Intent notificationIntent = new Intent(this, ServerBrowser.class);
		PendingIntent contentIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0);

		oNotification.setLatestEventInfo(context, contentTitle, contentText, contentIntent);
		
		final int NOTIFICATION_STARTED_ID = 1;

		oNM.notify(NOTIFICATION_STARTED_ID, oNotification);
		try {
			Socket oSock = new Socket(insHost,innPort);
			if( DoHandshake( oSock ) ) {
				DBHelper oHelper = new DBHelper( this );
	        	SQLiteDatabase oDB = oHelper.getReadableDatabase();
	        	SQLiteStatement oInsertStatement = oDB.compileStatement("INSERT INTO folders(IDOnServer,ServerID,Name,ServerPath,SyncToPhone,LocalPath) VALUES(?," + innServerID + ",?,?,1,'/mnt/sdcard/Syncro/')");
				GetFolderList(oSock,oInsertStatement);
				GetFolderContents(oSock,innServerID,oDB);
				oDB.close();
				oDB = null;
			}
			oSock.close();
			
			//TODO: Seperate notification code out into different function
			icon = R.drawable.stat_sys_warning;
			tickerText = "Syncro finished";
			when = System.currentTimeMillis();

			oNotification = new Notification(icon, tickerText, when);
			
			context = getApplicationContext();
			contentTitle = "Syncro Is Finished!";
			contentText = "Syncro is finished!";
			notificationIntent = new Intent(this, ServerBrowser.class);
			contentIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0);

			oNotification.setLatestEventInfo(context, contentTitle, contentText, contentIntent);
			
			final int NOTIFICATION_FINISHED_ID = 1;

			oNM.notify(NOTIFICATION_FINISHED_ID, oNotification);
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
	
	protected boolean GetFolderContents(Socket inoSock,int innServerID,SQLiteDatabase inoDB) throws IOException {
		String[] aArgs = new String[1];
		aArgs[0] = String.valueOf(innServerID);
		Cursor oFolders = inoDB.rawQuery("SELECT IDOnServer,LocalPath FROM folders WHERE ServerID=? AND SyncToPhone=1", aArgs);
		oFolders.moveToFirst();
		while (oFolders.isAfterLast() == false) {
            int nFolderID = (int)oFolders.getLong(0);
            m_sCurrentLocalPath = oFolders.getString(1);
            GetFolderContents(inoSock,nFolderID);
			GetFiles(inoSock);
			m_aFilesToDownload.clear();
            oFolders.moveToNext();
        }
		oFolders.close();
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
			oHandler.SetFolderId( innFolderID );
			ProcessXML(new ByteArrayInputStream(aData), oHandler );
			Log.i("Syncro","Files To Download:");
			for(int n=0;n < m_aFilesToDownload.size();n++) {
				Log.i("Syncro",m_aFilesToDownload.elementAt(n).Filename );
			}
		}
		return true;
	}
	
	@Override
	public void HandleRemoteFile(RemoteFileHandler.RemoteFileData inoFile) {
		m_aFilesToDownload.add(inoFile);
	}
	
	protected boolean GetFiles(Socket inoSock) throws IOException {

		boolean fOK = false;
		int nPrevFolderId = -1;
		
		for(int nFile = 0;nFile < m_aFilesToDownload.size();nFile++) {
			RemoteFileHandler.RemoteFileData oFile = m_aFilesToDownload.elementAt(nFile);
			if( (nPrevFolderId != -1) && (nPrevFolderId != oFile.FolderId) ) {
				//We have a changed folder id.  want to support both ways of working, so do something here if neccesary
			}
			String destFilename = GetDestinationFilename(oFile.FolderId, oFile.Filename);
			File oDestFile = new File(destFilename);
			if( oDestFile.length() != oFile.Size ) {
				fOK = GetFile( inoSock, oFile.FolderId, oFile.Filename );
				if( !fOK )
					return false;
			}
			nPrevFolderId = oFile.FolderId;
		}
		return fOK;
	}
	
	protected boolean GetFile(Socket inoSock,int innFolderId, String insFilename) throws IOException {
		boolean fOK = false;
		fOK = StartDownloadingFile(inoSock,innFolderId,insFilename);
		if( fOK ) {
			FileOutputStream oFile = new FileOutputStream( GetDestinationFilename( innFolderId,insFilename ) );
			try {
				fOK = ReceiveFile(inoSock,oFile);
			}catch (Exception e) {
				e.printStackTrace();
			}
			oFile.close();
		}
		return fOK;
	}
	
	protected String GetDestinationFilename(int innFolderId,String insFilename) throws IOException {
		if( m_sCurrentLocalPath == null )
			throw new IOException();		//TODO: Fix this to throw exception, just being really really lazy
		String insDestinationFolder = m_sCurrentLocalPath;
		//TODO: add code to get this from the database etc.
		//TODO: check for ..s or something?
		File oFile = new File( insDestinationFolder + insFilename );
		oFile.getParentFile().mkdirs();
		String rv = oFile.getCanonicalPath();
		return rv;
	}

	private boolean StartDownloadingFile(Socket inoSock,int innFolderId, String insFilename) throws IOException {
		Binarydata.BinaryDataRequest oRequest = Binarydata.BinaryDataRequest.newBuilder()
			.setFileName(insFilename)
			.setFolderId(innFolderId)
			.setRecvBufferSize( inoSock.getReceiveBufferSize() )
			.build();
		m_oPBInterface.SendObject(inoSock.getOutputStream(), PBSocketInterface.RequestTypes.BINARY_REQUEST ,oRequest);
		return true;
	}
	
	private boolean ReceiveFile(Socket inoSock, OutputStream inoFile) throws Exception {
		FileResponseHandler oResponseHandler = new FileResponseHandler(inoFile);
		InputStream oInputStream = inoSock.getInputStream();
		OutputStream oOutputStream = inoSock.getOutputStream();
		m_oPBInterface.addResponseHandler(oResponseHandler);
		boolean fDone = false;
		do {
			m_oPBInterface.HandleResponse(oInputStream);
			if( oResponseHandler.canRemove() ) {
				fDone = true;
			} else {
				m_oPBInterface.SendMessage(oOutputStream, PBSocketInterface.RequestTypes.BINARY_CONTINUE );
			}
		}while( !fDone );
		return true;
	}
}
