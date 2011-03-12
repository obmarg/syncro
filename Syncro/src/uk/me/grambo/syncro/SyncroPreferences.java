package uk.me.grambo.syncro;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

public class SyncroPreferences 
{
	private SharedPreferences m_prefs;
	
	public SyncroPreferences(Context context)
	{
		super();
		m_prefs = 
			PreferenceManager.getDefaultSharedPreferences(context);
	}
	
	public boolean OnlySyncOnWifi()
	{
		return m_prefs.getBoolean( "syncOnWifi", true );
	}
	
	public boolean UsageReporting()
	{
		return m_prefs.getBoolean( "sendUsageData", false );
	}
}
