package uk.me.grambo.syncro;

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
	
	private Notification m_oNotification;
	private Context m_oContext;
	
	private int m_nCurrentTotalSize;
	private String m_nFilename;
	private int m_nProgress;
	private boolean m_fHaveFile;
	
	private int m_nTotalFiles;
	private int m_nFileNum;
	
	private boolean m_fShowRate;
	private long m_nLastProgressUpdate;
	private float m_flCurrentRate;
	private float m_nDataSinceLast;
	
	private long m_nLastUpdateTime;
	
	private boolean m_downloading;
	
	protected ProgressNotification(Context inoContext) {
		m_oContext = inoContext;
		m_fHaveFile = false;
		m_nFilename = "";
		m_nCurrentTotalSize = 0;
		m_nFileNum = 0;
		m_nTotalFiles = 0;
		m_nProgress = 0;
		m_fShowRate = false;
		m_nLastProgressUpdate = 0;
		m_nLastUpdateTime = 0;
	}
	
	public void create(CharSequence tickerText) {
		int icon = R.drawable.stat_sys_warning;
		long when = System.currentTimeMillis();
		m_oNotification = new Notification(icon, tickerText, when);
		m_oNotification.flags = Notification.FLAG_ONGOING_EVENT;
		//TODO: could possibly make the notification a foreground event notification, but that's api level 5 >
		//		check it out etc.
	}
	
	public void setCurrentFileDetails(RemoteFileHandler.RemoteFileData inoFile, int innFileNum) {
		m_nFilename = inoFile.Filename;
		//TODO: Progress bar won't work for massive files - fix it
		m_nCurrentTotalSize = (int)inoFile.Size;
		m_fHaveFile = true;
		m_nFileNum = innFileNum;
		// Set the clock so we'll update in 1 second
		m_nLastProgressUpdate = SystemClock.uptimeMillis()-3000;
	}
	
	public void setCurrentFileDetails(String insFilename, int inFileSize, int innFileNum )
	{
		m_nFilename = insFilename;
		m_nCurrentTotalSize = inFileSize;
		m_fHaveFile = true;
		m_nFileNum = innFileNum;
		// Set the clock so we'll update in 1 second
		m_nLastProgressUpdate = SystemClock.uptimeMillis()-3000;
	}
	
	public void clearFileDetails() {
		m_fHaveFile = false;
	}
	
	public void setTotalNumFiles(int innTotalFiles) {
		m_nTotalFiles = innTotalFiles;
	}
	
	public void setProgress( int innProgress ) {
		long nCurrentTime = SystemClock.uptimeMillis();
		if( m_fShowRate ) {
			m_nDataSinceLast += (innProgress - m_nProgress);
			if( (m_nLastProgressUpdate == 0) || ( nCurrentTime > (m_nLastProgressUpdate+4000) ) ) {
				float flDuration = (nCurrentTime - m_nLastProgressUpdate);
				if( flDuration != 0 ) {
					//Stop divide from 0 from happening
					//TODO: support scaling from bytes > kb > mb depending on size
					float flDataSinceLast = m_nDataSinceLast / 1024;
					m_flCurrentRate = (flDataSinceLast / (flDuration/1000) );
				}
				m_nDataSinceLast = 0;
				m_nLastProgressUpdate = nCurrentTime;
			}
			if( m_flCurrentRate == 0 ) {
				//TODO: We've probably just started, but don't want it sitting at 0kb/s for 4 seconds, so do something here
			}
		}
		m_nProgress = innProgress;
		if( nCurrentTime > (m_nLastUpdateTime + UPDATE_RATE) ) {
			update();
		}
	}
	
	public void setShowRate( boolean innShowRate ) {
		m_fShowRate = innShowRate;
	}
	
	public void update() {
		//TODO: remove most calls to update, and just call setprogress instead 
		if( m_oNotification == null ) {
			create("Syncro Syncing!");
		}
		Intent notificationIntent = new Intent(m_oContext, ServerBrowser.class);
		PendingIntent contentIntent = PendingIntent.getActivity(m_oContext, 0, notificationIntent, 0);
		m_oNotification.contentIntent = contentIntent;

		//m_oProgressNotification.setLatestEventInfo(context, contentTitle, contentText, contentIntent);
		
		RemoteViews contentView = new RemoteViews(m_oContext.getPackageName(), R.layout.progress_notification);
		contentView.setImageViewResource(R.id.progress_notification_icon, R.drawable.stat_sys_warning);
		if( !m_fHaveFile ) {
			contentView.setTextViewText(R.id.progress_notification_text, "Syncro: Getting File Data");
		} else {
			String sRate = "";
			if( m_fShowRate ) {
				sRate = "(" + Integer.toString( new Float(m_flCurrentRate).intValue() ) + " kb/s)";
			}
			contentView.setTextViewText(R.id.progress_notification_text, "Syncro: Syncing File " + m_nFileNum + " of " + m_nTotalFiles + sRate);
		}
		contentView.setProgressBar(R.id.progress_notification_progress, m_nCurrentTotalSize, m_nProgress, !m_fHaveFile);
		m_oNotification.contentView = contentView;

		NotificationManager oNM = (NotificationManager)m_oContext.getSystemService(Context.NOTIFICATION_SERVICE);
		oNM.notify(PROGRESS_NOTIFICATION_ID, m_oNotification);
		m_nLastUpdateTime = SystemClock.uptimeMillis(); 
	}
	
	public void stop() {
		NotificationManager oNM = (NotificationManager)m_oContext.getSystemService(Context.NOTIFICATION_SERVICE);
		oNM.cancel(PROGRESS_NOTIFICATION_ID);
		m_oNotification = null;
	}
}
