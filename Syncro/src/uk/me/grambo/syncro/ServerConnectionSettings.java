package uk.me.grambo.syncro;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class ServerConnectionSettings extends Activity {

	public ServerConnectionSettings() {
		// TODO Auto-generated constructor stub
	}
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        TextView textview = new TextView(this);
        textview.setText("This is the Artists tab");
        setContentView(textview);
    }

}
