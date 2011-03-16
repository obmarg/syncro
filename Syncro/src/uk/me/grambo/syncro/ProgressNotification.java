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

import uk.me.grambo.syncro.comms.RemoteFileData;
import uk.me.grambo.syncro.ui.ServerBrowser;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.SystemClock;
import android.widget.RemoteViews;

public class ProgressNotification {
	
	final static int PROGRESS_NOTIFICATION_ID = 1;
	final static int UPDATE_RATE = 500;
	final static int CALCULATION_RATE = 2000;
	final static int MAX_FILENAME_SIZE = 34;
	
	private Notification m_notification;
	private Context m_context;
	
	private String m_filename;
	private int m_currentTotalSize;
	private int m_progress;
	private boolean m_uploading;
	private boolean m_haveFile;
	
	private int m_totalFiles;
	private int m_fileNum;
	
	private boolean m_showRate;
	private long m_lastProgressCalculation;
	private float m_currentRate;
	private String m_currentUnits;
	private float m_dataSinceLast;
	
	private long m_lastUpdateTime;
	
	private boolean m_downloading;
	
	protected ProgressNotification(Context inoContext) {
		m_context = inoContext;
		m_haveFile = false;
		m_filename = "";
		m_currentTotalSize = 0;
		m_fileNum = 0;
		m_totalFiles = 0;
		m_progress = 0;
		m_showRate = false;
		m_lastProgressCalculation = 0;
		m_lastUpdateTime = 0;
		m_uploading = false;
	}
	
	public void create(CharSequence tickerText) {
		int icon = R.drawable.stat_sys_warning;
		long when = System.currentTimeMillis();
		m_notification = new Notification(icon, tickerText, when);
		m_notification.flags = Notification.FLAG_ONGOING_EVENT;
		//TODO: could possibly make the notification a foreground event notification, but that's api level 5 >
		//		check it out etc.
	}
	
	public void setCurrentFileDetails(
			RemoteFileData file, 
			int fileNum,
			boolean uploading
			) 
	{
		setFilename( file.Filename );
		//TODO: Progress bar won't work for massive files - fix it
		m_currentTotalSize = (int)file.Size;
		m_haveFile = true;
		m_fileNum = fileNum;
		m_uploading = uploading;
		resetTimes();
	}
	
	public void setCurrentFileDetails(
			String filename, 
			int fileSize, 
			int fileNum,
			boolean uploading
			)
	{
		setFilename( filename );
		m_currentTotalSize = fileSize;
		m_haveFile = true;
		m_fileNum = fileNum;
		m_uploading = uploading;
		resetTimes();
	}
	
	private void setFilename( String filename )
	{
		//TODO: This could all be handled better, but it's getting late
		int maxFilenameSize = MAX_FILENAME_SIZE;
		maxFilenameSize -= getFileCountString().length();
		if( filename.length() > maxFilenameSize )
		{
			m_filename = 
				filename.substring( 0, maxFilenameSize ) + "...";
		}
		else
		{
			m_filename = filename;
		}
	}
	
	private void resetTimes()
	{
		// Set the clock so we'll update in 0.5 second
		m_lastProgressCalculation = 
			SystemClock.uptimeMillis() - CALCULATION_RATE + 500;
		m_dataSinceLast = 0;
		m_lastUpdateTime = 
			SystemClock.uptimeMillis() - UPDATE_RATE + 510;
	}
	
	public void clearFileDetails() {
		m_haveFile = false;
	}
	
	public void setTotalNumFiles(int innTotalFiles) {
		m_totalFiles = innTotalFiles;
	}
	
	public void setProgress( int innProgress ) {
		long nCurrentTime = SystemClock.uptimeMillis();
		if( m_showRate ) {
			m_dataSinceLast += (innProgress - m_progress);
			if( 
				(m_lastProgressCalculation == 0) || 
				( nCurrentTime > (m_lastProgressCalculation+CALCULATION_RATE) ) 
				) 
			{
				float flDuration = (nCurrentTime - m_lastProgressCalculation);
				if( flDuration != 0 ) {
					//Stop divide from 0 from happening
					float flDataSinceLast = m_dataSinceLast / 1000;
					m_currentRate = (flDataSinceLast / (flDuration/1000) );
					if( m_currentRate < 1 )
					{
						//Switch to bytes/s
						m_currentRate = (m_dataSinceLast / (flDuration/1000) );
						m_currentUnits = "Bps";
					}
					else if( m_currentRate > 1024 )
					{
						//Switch to MB/s
						flDataSinceLast = flDataSinceLast / 1000;
						m_currentRate = (flDataSinceLast / (flDuration/1000) );
						m_currentUnits = "MBps";
					}
					else
					{
						m_currentUnits = "kBps";
					}
				}
				m_dataSinceLast = 0;
				m_lastProgressCalculation = nCurrentTime;
			}
			if( m_currentRate <= 0 ) {
				m_lastProgressCalculation = 0;
				m_currentRate = 0;
			}
		}
		m_progress = innProgress;
		if( nCurrentTime > (m_lastUpdateTime + UPDATE_RATE) ) {
			update();
		}
	}
	
	public void setShowRate( boolean innShowRate ) {
		m_showRate = innShowRate;
	}
	
	public void update() {
		//TODO: remove most calls to update, and just call setprogress instead 
		if( m_notification == null ) {
			create("Syncro Syncing!");
		}
		Intent notificationIntent = new Intent(m_context, ServerBrowser.class);
		PendingIntent contentIntent = PendingIntent.getActivity(m_context, 0, notificationIntent, 0);
		m_notification.contentIntent = contentIntent;

		//m_oProgressNotification.setLatestEventInfo(context, contentTitle, contentText, contentIntent);
		
		RemoteViews contentView = new RemoteViews(m_context.getPackageName(), R.layout.progress_notification);
		contentView.setImageViewResource(R.id.progress_notification_icon, R.drawable.stat_sys_warning);
		if( !m_haveFile ) {
			contentView.setTextViewText(R.id.progress_notification_text, "Syncro: Getting File Data");
		} else {
			String sRate = "";
			if( m_showRate ) {
				sRate = Integer.toString( new Float(m_currentRate).intValue() ) + m_currentUnits;
			}
			String directionWord;
			if( m_uploading )
			{
				directionWord = "Uploading ";
			}
			else
			{
				directionWord = "Downloading ";
			}
			String fileCount = getFileCountString();
			contentView.setTextViewText(
					R.id.progress_notification_text, 
					directionWord + m_filename +
					fileCount + "\n" +
					sRate
					);
		}
		contentView.setProgressBar(R.id.progress_notification_progress, m_currentTotalSize, m_progress, !m_haveFile);
		m_notification.contentView = contentView;

		NotificationManager oNM = (NotificationManager)m_context.getSystemService(Context.NOTIFICATION_SERVICE);
		oNM.notify(PROGRESS_NOTIFICATION_ID, m_notification);
		m_lastUpdateTime = SystemClock.uptimeMillis(); 
	}
	
	public void stop() {
		NotificationManager oNM = (NotificationManager)m_context.getSystemService(Context.NOTIFICATION_SERVICE);
		oNM.cancel(PROGRESS_NOTIFICATION_ID);
		m_notification = null;
	}
	
	private String getFileCountString()
	{
		if( m_totalFiles > 1 )
		{
			return " (" + m_fileNum + "/" + m_totalFiles + ")";
		}
		else
		{
			return "";
		}
	}
}
