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

package uk.me.grambo.syncro.ui;

import uk.me.grambo.syncro.DBHelper;
import uk.me.grambo.syncro.R;
import uk.me.grambo.syncro.R.id;
import uk.me.grambo.syncro.R.layout;
import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class FolderFilterSettings extends Activity {
	
	protected enum eFilterTypes {
		IncludeFilter,
		FilenameFilter
	}
	
	protected final int NUM_FILTER_TYPES = 2;
	
	private int m_nServerID;
	private int m_nFolderID;
	
	private String 	m_aFilters[][];
	private int 	m_aFilterIDs[][];

	public FolderFilterSettings() {
		m_aFilters = new String[NUM_FILTER_TYPES][];
		m_aFilterIDs = new int[NUM_FILTER_TYPES][];
	}
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
		Intent parameterIntent = getIntent();
		m_nServerID = parameterIntent.getExtras().getInt("uk.me.grambo.syncro.server_id");
		m_nFolderID = parameterIntent.getExtras().getInt("uk.me.grambo.syncro.folder_id");

        setContentView(R.layout.folder_filter_list);
        
        ListView oIncludeFilterList = (ListView)findViewById(R.id.folder_filter_list_include_filters);
        ListView oFilenameFilterList = (ListView)findViewById(R.id.folder_filter_list_filename_filters);
        
        //TODO: Add click handlers
        try {
        	DBHelper oHelper = new DBHelper( this );
        	SQLiteDatabase oDB = oHelper.getReadableDatabase();
        	fillFilterList( oDB, oIncludeFilterList, eFilterTypes.IncludeFilter );
        	fillFilterList( oDB, oFilenameFilterList, eFilterTypes.FilenameFilter );
	        oDB.close();
        }catch(SQLException oException) {
        	oException.printStackTrace();
        }
    }
    
    protected void fillFilterList ( SQLiteDatabase inoDB, ListView inoList, eFilterTypes ineType ) {
    	int nFilterType;
    	if( ineType == eFilterTypes.IncludeFilter )
    		nFilterType = 1;
    	else
    		nFilterType = 2;
    	String aArgs[] = { Integer.toString(nFilterType), Integer.toString(m_nFolderID) };
    	Cursor oFilters = inoDB.rawQuery("SELECT ID,Name FROM filters WHERE FilterType=? AND FolderID=?", aArgs );
    	if( oFilters.moveToFirst() ) {
    		m_aFilters[ nFilterType ] = new String[ oFilters.getCount() ];
    		m_aFilterIDs[ nFilterType ] = new int[ oFilters.getCount() ];
    		int nCount=0;
    		do {
    			m_aFilters[ nFilterType ][nCount] = oFilters.getString( 1 );
    			m_aFilterIDs[ nFilterType ][nCount] = oFilters.getInt( 0 );
    			nCount++;
    		}while( oFilters.moveToNext() );
    		inoList.setAdapter( new ArrayAdapter<String>(this, R.layout.list_item, m_aFilters[ nFilterType ] ) );
    	}
    }

}
