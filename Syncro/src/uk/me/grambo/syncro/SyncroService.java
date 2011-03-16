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

package uk.me.grambo.syncro;

import java.io.EOFException;
import java.io.File;
import java.io.IOException;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Vector;

import uk.me.grambo.syncro.comms.Connection;
import uk.me.grambo.syncro.comms.ConnectionDetails;
import uk.me.grambo.syncro.comms.FolderContentsHandler;
import uk.me.grambo.syncro.comms.FolderListHandler;
import uk.me.grambo.syncro.comms.ProgressHandler;
import uk.me.grambo.syncro.comms.RemoteFileData;
import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteStatement;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.PowerManager;
import android.util.Log;

public class SyncroService 
extends IntentService 
implements FolderContentsHandler,FolderListHandler, ProgressHandler
{
	
	private Vector<RemoteFileData> m_filesToDownload;
	
	private FilterFactory m_oFilterFactory;
	private ArrayList<IncludeFilter> m_includeFilters;
	private ArrayList<FilenameFilter> m_filenameFilters;
	
	private ProgressNotification m_progressNotification;
	
	private SQLiteDatabase 	m_db;
	private SQLiteStatement m_folderInsertStatement;
	
	private Connection		m_conn;
	
	private ArrayList<Integer> m_currentFolderIDs;
	
	String m_sCurrentLocalPath;
	
	private int	m_serverId;

	public SyncroService() {
		super("SyncroService");
		// TODO Auto-generated constructor stub
		m_filesToDownload = new Vector<RemoteFileData>();
		m_includeFilters = new ArrayList<IncludeFilter>();
		m_filenameFilters = new ArrayList<FilenameFilter>();
		m_oFilterFactory = new FilterFactory(this);
		m_conn = new Connection(this);
		m_currentFolderIDs = new ArrayList<Integer>(30);
	}

	@Override
	protected void onHandleIntent(Intent intent) {
		if( intent.getAction().equals("uk.me.grambo.syncro.SYNCRO_SYNC") ) {
			
			SyncroPreferences prefs = new SyncroPreferences(this);
			if( prefs.OnlySyncOnWifi() )
			{
				WifiManager wifiMan = (WifiManager)getSystemService(Context.WIFI_SERVICE);
				if( !wifiMan.isWifiEnabled() )
				{
					return;
				}
				WifiInfo wifiInfo = wifiMan.getConnectionInfo();
				if( wifiInfo == null )
					return;
				String ssid = wifiInfo.getSSID(); 
				if( ssid == null )
					return;
			}
			
			Uri oURI = intent.getData();
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
			    		boolean retry;
			    		int retryCount = 5;
			    		do {
			    			retry = !RunSync( nID, sAddress, nPort );
			    			retryCount--;
			    			if( retry && ( retryCount > 0 ) )
			    			{
			    				Log.w("Syncro", "Syncro failed.  Retrying in 10 seconds");
				    			try {
									Thread.sleep( 10000 );
								} catch (InterruptedException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
									retry = false;
								}
								Log.i("Syncro", "Retrying now...");
			    			}else if( retry )
			    			{
			    				Log.e("Syncro", "Syncro failed, but ran out of retries");
			    			}
			    		}while( retry && ( retryCount > 0 ) );
			    	} else {
			    		oDB.close();
			    	}
				}
			}
		}
	}
	
	/**
	 * Runs an entire sync with the server specified
	 * @param innServerID	The ID of the server in the database
	 * @param insHost		The hostname of the server
	 * @param innPort		The port of the server
	 * @return				True if we should not retry, false if we should
	 */
	protected boolean RunSync(int innServerID,String insHost,int innPort) {
		boolean retry = false;
		boolean gotInitialConnection = false;
		
		m_serverId = innServerID;
		
		m_progressNotification = new ProgressNotification(this);
		m_progressNotification.setShowRate(true);

		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "Syncro");
		 wl.acquire();
		try {
			ConnectionDetails details = 
				ConnectionDetails.createInstance()
				.setHostname(insHost)
				.setPort( innPort );
			
			boolean handshakeOk = m_conn.Connect(details);
			
			gotInitialConnection = true;
			
			//Start the progress notification
			m_progressNotification.update();
			if( handshakeOk ) {
				DBHelper oHelper = new DBHelper( this );
				m_db = oHelper.getWritableDatabase();
	        	m_folderInsertStatement = 
	        		m_db.compileStatement(
	        				"INSERT INTO folders" +
	        				"(IDOnServer,ServerID,Name,ServerPath,LocalPath) " +
	        				"VALUES(?," + innServerID + ",?,?,'/mnt/sdcard/Syncro/')"
	        				);
	        	m_currentFolderIDs.clear();
	        	m_conn.GetFolderList( this );
	        	MarkMissingFoldersAsDeleted();
	        	m_currentFolderIDs.clear();
	        	
	        	//
	        	//	Send a folder list update broadcast to update the UI
	        	//
	        	Intent broadcast = new Intent();
	        	broadcast.setAction("uk.me.grambo.syncro.ui.FOLDER_LIST_UPDATE");
	        	//TODO: Uncomment this next line and add in data to send the folder id
	        	//broadcast.setData("syncro://folderid=");
	        	sendBroadcast( broadcast );
	        	
	        	PerformDownloads( );
	        	PerformUploads( );
	        	
			}
		} 
		catch ( SocketException e )
		{
			if( gotInitialConnection )
				retry = true;
			else {
				Log.e("Syncro","Couldn't connect to server");
				e.printStackTrace();
			}
			//TODO: Need to work out the difference between can't connect at all,
			//		and have been disconnected
		}
		catch( EOFException e )
		{
			//TODO: Need to determine between network EOF and file EOF
			if( gotInitialConnection )
				retry = true;
			else {
				Log.e("Syncro","Couldn't connect to server");
				e.printStackTrace();
			}
		}
		catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			
			//TODO: update notification or something here?
		} catch( Exception e ) {
			e.printStackTrace();
		}
		finally
		{
			if( m_db != null )
			{
				m_db.close();
				m_db = null;
			}	
			m_conn.Disconnect();
			m_progressNotification.stop();
			wl.release();
		}
		return !retry;
	}
	
	//
	// Marks any folders for this server not contained in
	// m_currentFolderIDs as deleted
	//
	private void MarkMissingFoldersAsDeleted() {
		// TODO Auto-generated method stub
		String query = 
			"UPDATE folders SET Deleted=1 WHERE " +
			"ServerID=" + m_serverId +  
			" AND IDOnServer NOT IN (";
		boolean first = true;
		for( Integer id : m_currentFolderIDs )
		{
			if( first )
			{
				first = false;
			}
			else
			{
				query += ", ";
			}
			query += String.valueOf( id );
		}
		query += ");";
		m_db.execSQL( query );
	}

	//
	//	Remote file/folder handler callbacks
	//
	@Override
	public void handleRemoteFile(RemoteFileData inoFile) {
		if( CheckIncludeFilters(inoFile) )
			m_filesToDownload.add(inoFile);
	}
	
	@Override
	public void handleFolder(FolderInfo folder) {
		m_folderInsertStatement.bindLong( 1, folder.Id );
		m_folderInsertStatement.bindString( 2,  folder.Name );
		m_folderInsertStatement.bindString( 3,  folder.Path );
		m_folderInsertStatement.executeInsert();
		m_currentFolderIDs.add( folder.Id );
	}
	
	//
	// Progress Handler Callbacks
	//
	
	@Override
	public void setTotalProgress(long total) {
		//TODO: Implement me
	}

	@Override
	public void setCurrentProgress(long progress) {
		// TODO: Fix the long/int conversion in here
		m_progressNotification.setProgress( (int)progress );
	}
	
	//
	// File downloading functions
	//
	
	/**
	 * Gets folder contents, and downloads any neccesary files
	 * @throws Exception 
	 */
	protected void PerformDownloads() throws Exception
	{
		String[] queryArgs = new String[1];
		
		queryArgs[0] = String.valueOf( m_serverId );
		Cursor folders = 
			m_db.rawQuery(
					"SELECT IDOnServer,LocalPath " +
					"FROM folders WHERE ServerID=? " +
					"AND SyncToPhone=1 " +
					"AND Deleted=0", 
					queryArgs
					);
		folders.moveToFirst();
		while (folders.isAfterLast() == false) {
			int folderId = (int)folders.getLong(0);
			m_sCurrentLocalPath = folders.getString(1);
			
			m_includeFilters.clear();
			m_oFilterFactory.getIncludeFilters( 
					m_db, 
					folderId, 
					m_includeFilters 
					);
			
			m_conn.GetFolderContents( folderId, this );
			if( m_filesToDownload.size() > 0 ) {
				m_filenameFilters.clear();
				m_oFilterFactory.getFilenameFilters( 
						m_db, 
						folderId,
						m_filenameFilters 
						);
				GetFiles();
			}
		
			//TODO: Tidy up excess clears sometime?
			//		(if there are any)
			m_filesToDownload.clear();
			m_includeFilters.clear();
			m_filenameFilters.clear();
			
			queryArgs = new String[3];
			queryArgs[0] = String.valueOf( System.currentTimeMillis() );
			queryArgs[1] = String.valueOf( m_serverId );
			queryArgs[2] = String.valueOf( folderId );
			
			m_db.rawQuery(
				"UPDATE folders SET LastSyncTime=? " +
				"WHERE ServerID=? AND IDOnServer=?;",
				queryArgs
				);
		
			folders.moveToNext();
		}
		folders.close();
	}
	
	/**
	 * Attempts to download all the files in the files to download array
	 * @return		True on success, false on failure
	 * @throws Exception
	 * @throws IOException
	 */
	private boolean GetFiles() throws Exception,IOException {
		//android.os.Debug.startMethodTracing("syncro-download");
		boolean fOK = false;
		int nPrevFolderId = -1;
		m_progressNotification.setTotalNumFiles( m_filesToDownload.size() );
		for(int nFile = 0;nFile < m_filesToDownload.size();nFile++) {
			RemoteFileData oFile = m_filesToDownload.elementAt(nFile);

			if( (nPrevFolderId != -1) && (nPrevFolderId != oFile.FolderId) ) {
				throw new Exception("GetFiles called with contents of different folders");
			}
			
			String destFilename;
			try {
				destFilename = GetDestinationFilename(oFile);
			}catch(Exception e) {
				e.printStackTrace();
				continue;
			}
			
			if( CheckIncludeFilters(oFile,destFilename) ) {
				//TODO: To get accurate total file numbers, need to run include filters etc. before this loop
				//			but never mind for now...
				m_progressNotification.setCurrentFileDetails( oFile, nFile );
				m_progressNotification.setProgress( 0 );
				fOK = m_conn.GetFile( oFile, destFilename, this );
				if( !fOK )
					return false;
			}
			
			nPrevFolderId = oFile.FolderId;
		}
		//android.os.Debug.stopMethodTracing();
		return fOK;
	}
	
	//
	// File download utility functions
	//
	
	/**
	 * GetDestinationFilename
	 * @return	Returns the destination filename of the specified remote file data
	 */
	private String GetDestinationFilename(RemoteFileData inoFile) throws Exception {
		for( int n=0; n < m_filenameFilters.size(); n++ ) {
			FilenameFilter oFilter = m_filenameFilters.get(n);
			if( oFilter.canHandle(inoFile) ) {
				return oFilter.getDestinationFilename(inoFile);
			}
		}
		throw new Exception("Could not find suitable Filename filter for " + inoFile.Filename + " (FolderID: " + inoFile.FolderId + ")");
	}
	
	/**
	 * Checks if we should download the file specified
	 * @param inoFile	The details of the file to check
	 * @return			True if we should download it
	 */
	private boolean CheckIncludeFilters(RemoteFileData inoFile) {
		for( int n=0; n < m_includeFilters.size(); n++ ) {
			IncludeFilter oFilter = m_includeFilters.get( n );
			if( !oFilter.needsFilename() ) {
				if( oFilter.shouldInclude(inoFile) ) {
					return true;
				} 
				//TODO: probably a better way to handle this should end list stuff...
				if( oFilter.shouldEndList() )
					return false;
			}
		}
		return false;
	}
	
	/**
	 * Checks again if we should download the file specified, based on dest filename
	 * @param inoFile				The details of the file to check
	 * @param insDestFilename		The destination filename
	 * @return						True if we should download the file
	 */
	private boolean CheckIncludeFilters(RemoteFileData inoFile,String insDestFilename) {
		for( int n=0; n < m_includeFilters.size(); n++ ) {
			IncludeFilter oFilter = m_includeFilters.get( n );
			if( oFilter.needsFilename() ) {
				if( oFilter.shouldInclude(inoFile,insDestFilename) ) {
					return true;
				} 
				//TODO: probably a better way to handle this should end list stuff...
				if( oFilter.shouldEndList() )
					return false;
			}
		}
		return false;
	}
	
	//
	// Send File functions
	//
	
	/**
	 * Runs the upload stage of a sync
	 */
	protected void PerformUploads() throws IOException,Exception {
		//android.os.Debug.startMethodTracing("syncro-upload");
		String args[] = { Integer.valueOf( m_serverId ).toString() };
		Cursor results = m_db.rawQuery(
				"SELECT ID,IDOnServer,LocalPath " +
				"FROM folders WHERE SyncFromPhone=1 AND ServerID=?", 
				args);
		//TODO: use the actual number of files here....
		m_progressNotification.setTotalNumFiles(1);
		while( results.moveToNext() )
		{
			String folderPath = results.getString(2);
			File folder = new File(folderPath);
			if( !folder.isDirectory() || !folder.canRead() )
			{
				continue;
			}
			m_sCurrentLocalPath = folder.getAbsolutePath();
			if( !m_sCurrentLocalPath.endsWith( File.separator ) )
				m_sCurrentLocalPath = 
					m_sCurrentLocalPath.concat( File.separator );
			SendFolder( results.getInt(1), folder );
			SendOneShotFiles( results.getInt(0) );
		}
		//android.os.Debug.stopMethodTracing();
	}

	/**
	 * Processes a folder, sending all it's contents to the server
	 * @param folderIdOnServer	The current folderID we're using
	 * @param folder			The File object that represents this older
	 * @throws Exception
	 */
	private void SendFolder(int folderIdOnServer,File folder) throws Exception
	{
		File[] files = folder.listFiles();
		
		for( int nFile=0; nFile < files.length; nFile++ )
		{
			if( files[nFile].isDirectory() )
			{
				SendFolder( folderIdOnServer, files[ nFile ] );
			}
			else
			{
				String sendPath = 
					files[ nFile ].getAbsolutePath().substring( 
											m_sCurrentLocalPath.length() 
											);
				m_progressNotification.setCurrentFileDetails(
						sendPath, 
						(int)files[ nFile ].length(), 
						1
						);
				m_conn.SendFile(
						folderIdOnServer, 
						files[ nFile ].getAbsolutePath(), 
						sendPath, 
						this 
						);
			}
		}
	}
	
	/**
	 * Sends all the one shot files from db to server
	 * @param The ID of the folder to upload files for
	 * @throws Exception 
	 */
	private void SendOneShotFiles( int folderId ) throws Exception 
	{	
		try 
		{
			String[] args = { String.valueOf( folderId ) };
			Cursor results = m_db.rawQuery(
					"SELECT u.Filename,f.IDOnServer,u.ID FROM " +
					"uploads AS u LEFT JOIN folders AS f ON " +
					"u.FolderID=f.ID WHERE u.FolderID=?",
					args
					);
			
			if( !results.moveToFirst() )
			{
				results.close();
				return;
			}
			
			String deleteWhere = "(";
			boolean first = true;
			do
			{
				if( first )
				{
					first = false;
				}
				else
				{
					deleteWhere += ", ";
				}
				String fileName = results.getString( 0 );
				int idOnServer = results.getInt( 1 );
				deleteWhere += results.getInt( 2 );
				File file = new File( fileName );
				String sendFileName = file.getName();
				m_progressNotification.setCurrentFileDetails(
					sendFileName,
					(int)file.length(),
					1
					);
				m_conn.SendFile(
					idOnServer, 
					fileName, 
					sendFileName, 
					this
					);
			}while( results.moveToNext() );
			m_db.execSQL(
				"DELETE FROM uploads WHERE ID IN "
				+ deleteWhere + " )"
				);
		}
		catch( SQLException e )
		{
			e.printStackTrace();
		}
		
	}
	
}
