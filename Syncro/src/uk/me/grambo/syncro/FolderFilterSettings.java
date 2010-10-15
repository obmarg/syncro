package uk.me.grambo.syncro;

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
