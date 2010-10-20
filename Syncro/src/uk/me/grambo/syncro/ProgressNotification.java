package uk.me.grambo.syncro;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.widget.RemoteViews;

public class ProgressNotification {
	private Notification m_oNotification;
	private Context m_oContext;
	
	private int m_nCurrentTotalSize;
	private String m_nFilename;
	private int m_nProgress;
	private boolean m_fHaveFile;
	
	private int m_nTotalFiles;
	private int m_nFileNum;
	
	protected ProgressNotification(Context inoContext) {
		m_oContext = inoContext;
		m_fHaveFile = false;
		m_nFilename = "";
		m_nCurrentTotalSize = 0;
		m_nTotalFiles = 0;
		m_nProgress = 0;
	}
	
	public void create(CharSequence tickerText) {
		int icon = R.drawable.stat_sys_warning;
		long when = System.currentTimeMillis();
		m_oNotification = new Notification(icon, tickerText, when);
	}
	
	public void setCurrentFileDetails(RemoteFileHandler.RemoteFileData inoFile, int innFileNum) {
		m_nFilename = inoFile.Filename;
		m_nCurrentTotalSize = inoFile.Size;
		m_fHaveFile = true;
		m_nFileNum = innFileNum;
	}
	
	public void clearFileDetails() {
		m_fHaveFile = false;
	}
	
	public void setTotalNumFiles(int innTotalFiles) {
		m_nTotalFiles = innTotalFiles;
	}
	
	public void setProgress( int innProgress ) {
		m_nProgress = innProgress;
	}
	
	public void update() {
		if( m_oNotification == null )
			create("Syncro Syncing!");
		Intent notificationIntent = new Intent(m_oContext, ServerBrowser.class);
		PendingIntent contentIntent = PendingIntent.getActivity(m_oContext, 0, notificationIntent, 0);
		m_oNotification.contentIntent = contentIntent;

		//m_oProgressNotification.setLatestEventInfo(context, contentTitle, contentText, contentIntent);
		
		RemoteViews contentView = new RemoteViews(m_oContext.getPackageName(), R.layout.progress_notification);
		contentView.setImageViewResource(R.id.progress_notification_icon, R.drawable.stat_sys_warning);
		if( !m_fHaveFile ) {
			contentView.setTextViewText(R.id.progress_notification_text, "Syncro: Getting File Data");
		} else {
			contentView.setTextViewText(R.id.progress_notification_text, "Syncro: Syncing File " + m_nFileNum + " of " + m_nTotalFiles + " : " + m_nFilename);
		}
		contentView.setProgressBar(R.id.progress_notification_progress, m_nCurrentTotalSize, m_nProgress, !m_fHaveFile);
		m_oNotification.contentView = contentView;
		
		final int PROGRESS_NOTIFICATION_ID = 1;

		NotificationManager oNM = (NotificationManager)m_oContext.getSystemService(Context.NOTIFICATION_SERVICE);
		//TODO: do we need to notify, or can we just update the existing notification?
		oNM.notify(PROGRESS_NOTIFICATION_ID, m_oNotification);
	}
}
