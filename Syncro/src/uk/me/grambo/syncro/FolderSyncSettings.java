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
