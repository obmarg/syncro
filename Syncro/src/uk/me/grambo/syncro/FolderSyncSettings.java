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

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;

public class FolderSyncSettings extends Activity {
	
	private int m_nServerID;
	private int m_nFolderID;

	public FolderSyncSettings() {
		// TODO Auto-generated constructor stub
	}
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
		Intent parameterIntent = getIntent();
		m_nServerID = parameterIntent.getExtras().getInt("uk.me.grambo.syncro.server_id");
		m_nFolderID = parameterIntent.getExtras().getInt("uk.me.grambo.syncro.folder_id");

        TextView textview = new TextView(this);
        textview.setText("This is the Folder Sync Settings tab");
        setContentView(textview);
    }

}
