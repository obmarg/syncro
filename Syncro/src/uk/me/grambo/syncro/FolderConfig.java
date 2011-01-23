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

import android.app.TabActivity;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.widget.TabHost;

public class FolderConfig extends TabActivity {
	
	private int m_nServerID;
	private int m_nFolderID;

	public FolderConfig() {
		// TODO Auto-generated constructor stub
	}

	public void onCreate(Bundle savedInstanceState) {
		
	    super.onCreate(savedInstanceState);
	    
		Intent parameterIntent = getIntent();
		m_nFolderID = parameterIntent.getExtras().getInt("uk.me.grambo.syncro.folder_id");
		m_nServerID = parameterIntent.getExtras().getInt("uk.me.grambo.syncro.server_id");
		
	    setContentView(R.layout.folder_config);

	    Resources res = getResources(); // Resource object to get Drawables
	    TabHost tabHost = getTabHost();  // The activity TabHost
	    TabHost.TabSpec spec;  // Resusable TabSpec for each tab
	    Intent intent;  // Reusable Intent for each tab

	    // Create an Intent to launch an Activity for the tab (to be reused)
	    intent = new Intent().setClass(this, FolderGeneralSettings.class);
	    intent.putExtra("uk.me.grambo.syncro.server_id", m_nServerID);
	    intent.putExtra("uk.me.grambo.syncro.folder_id", m_nFolderID);

	    
	    // Initialize a TabSpec for each tab and add it to the TabHost
	    spec = tabHost.newTabSpec("general").setIndicator("General",
	                      res.getDrawable(R.drawable.ic_tab_connection))
	                  .setContent(intent);
	    tabHost.addTab(spec);
	    
	    intent = new Intent().setClass(this, FolderSyncSettings.class);
	    intent.putExtra("uk.me.grambo.syncro.server_id", m_nServerID);
	    intent.putExtra("uk.me.grambo.syncro.folder_id", m_nFolderID);
	    
	    spec = tabHost.newTabSpec("syncs").setIndicator("Sync Rules",
                res.getDrawable(R.drawable.ic_tab_connection))
            .setContent(intent);
	    tabHost.addTab(spec);
	    
	    intent = new Intent().setClass(this, FolderFilterSettings.class);
	    intent.putExtra("uk.me.grambo.syncro.server_id", m_nServerID);
	    intent.putExtra("uk.me.grambo.syncro.folder_id", m_nFolderID);
	    
	    spec = tabHost.newTabSpec("filters").setIndicator("Filters",
                res.getDrawable(R.drawable.ic_tab_connection))
            .setContent(intent);
	    tabHost.addTab(spec);

	    tabHost.setCurrentTab(0);
	}
}
