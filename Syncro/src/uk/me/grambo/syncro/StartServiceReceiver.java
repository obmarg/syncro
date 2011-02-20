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

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteStatement;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;

public class StartServiceReceiver extends BroadcastReceiver {

	public StartServiceReceiver() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO At some point, check the contents of the intent
		//		but for now, don't bother, as we're only registered for one thing anyway
		Bundle extras = intent.getExtras();
		if( extras != null ) {
			int serverId = extras.getInt("Id", -1);
			if( serverId == -1 ) {
				String serverName = extras.getString("ServerName");
				Log.d("Syncro", "Attempting to find server: " + serverName);
				DBHelper oHelper = new DBHelper( context );
	        	SQLiteDatabase oDB = oHelper.getReadableDatabase();
	        	String aArgs[] = {serverName};
	        	Cursor results = oDB.rawQuery(
	        			"SELECT ID FROM servers WHERE name=?",aArgs );
	        	if( results.moveToFirst() )
	        	{
	        		serverId = results.getInt(0);
	        	}
	        	else
	        	{
	        		Log.e("Syncro", "Could not find server.");
	        	}
	        	results.close();
	        	oDB.close();
			}
			if( serverId != -1 )
			{
				Log.i("Syncro","Starting sync with server Id:" + serverId);
				Intent i = new Intent( context, SyncroService.class );
				i.setAction("uk.me.grambo.syncro.SYNCRO_SYNC");
				i.setData( Uri.parse( "syncroid://" + serverId ) );
				context.startService( i );
			}
		}
		//
	}

}
