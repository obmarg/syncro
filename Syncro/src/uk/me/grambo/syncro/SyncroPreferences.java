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
	
	public boolean ShowHelpDialogs()
	{
		return m_prefs.getBoolean( "showHelpDialogs", true );
	}
	
	public boolean ShowHelp(String dialogName)
	{
		boolean rv = false;
		if( ShowHelpDialogs() )
		{
			String preferenceName = "showHelp" + dialogName;
			rv = m_prefs.getBoolean( preferenceName, true );
			SetPreference( preferenceName, false );
			return rv;
		}
		return rv;		
	}
	
	private void SetPreference(
		String preferenceName, 
		boolean value
		)
	{
		SharedPreferences.Editor edit = 
			m_prefs.edit();
		edit.putBoolean( preferenceName, value );
		edit.commit();
	}
}
