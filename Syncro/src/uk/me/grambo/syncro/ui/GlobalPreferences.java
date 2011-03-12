package uk.me.grambo.syncro.ui;

import uk.me.grambo.syncro.R;
import android.os.Bundle;
import android.preference.PreferenceActivity;

public class GlobalPreferences extends PreferenceActivity {
	public GlobalPreferences()
	{
		super();
	}
	
	protected void onCreate(Bundle savedInstanceState) 
	{
         super.onCreate(savedInstanceState);
         addPreferencesFromResource( R.xml.global_preferences );
	}
}
