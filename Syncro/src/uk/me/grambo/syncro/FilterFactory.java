package uk.me.grambo.syncro;

import java.util.ArrayList;

import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import uk.me.grambo.syncro.filters.filename.*;
import uk.me.grambo.syncro.filters.include.AlwaysIncludeFilter;

public class FilterFactory {

	protected Context m_oContext;
	
	public FilterFactory(Context inoContext) {
		m_oContext = inoContext;
	}
	
	private static final int FileTimeIncludeFilter = 1;
	private static final int CloneFilenameFilter = 2;
	
	public void getIncludeFilters( SQLiteDatabase inoDB, int innFolderID, ArrayList<IncludeFilter> inoList ) throws Exception {
		String aArgs[] = { Integer.toString(innFolderID), Integer.toString(1), Integer.toString(2) };
		Cursor oResults = inoDB.rawQuery("SELECT FilterType,ID FROM filters WHERE FolderId=? AND (IncludeType=? OR IncludeType=?)", aArgs);
		if( oResults.moveToFirst() ) {
			do {
				IncludeFilter oItem = getIncludeFilter( oResults.getInt( 0 ), oResults.getInt( 1 ) ); 
				inoList.add( oItem );
			}while( oResults.moveToNext() );
		} else {
			//TODO: Remove this after the GUI filter management is finished
			inoList.add( getIncludeFilter( FileTimeIncludeFilter, 0 ) );
		}
		oResults.close();
	}
	
	public void getFilenameFilters( SQLiteDatabase inoDB, int innFolderID, ArrayList<FilenameFilter> inoList ) throws Exception {
		String aArgs[] = { Integer.toString(innFolderID), Integer.toString(1) };
		Cursor oResults = inoDB.rawQuery("SELECT FilterType,ID FROM filters WHERE FolderId=? AND FilenameType=?", aArgs);
		if( oResults.moveToFirst() ) {
			do {
				FilenameFilter oItem = getFilenameFilter( oResults.getInt( 0 ), oResults.getInt( 1 ) ); 
				inoList.add( oItem );
			}while( oResults.moveToNext() );
		} else {
			//TODO: Remove this after the GUI filter management is finished
			inoList.add( getFilenameFilter( CloneFilenameFilter, 0 ) );
		}
		oResults.close();
	}

	public IncludeFilter getIncludeFilter( int nFilterType, int nFilterId ) throws Exception {
		if( nFilterType == FileTimeIncludeFilter )
			return new AlwaysIncludeFilter();
		throw new Exception( "Invalid Include Filter Type Selected");
	}
	
	
	public FilenameFilter getFilenameFilter( int innFilterType, int innFilterId ) throws Exception {
		try {
    		DBHelper oHelper = new DBHelper( m_oContext );
    		SQLiteDatabase oDB = oHelper.getReadableDatabase();
    		if( innFilterType == CloneFilenameFilter ) {
    			String aArgs[] = { Integer.toString( innFilterId ), Integer.toString( innFilterType ) };
    			Cursor oResults = oDB.rawQuery("SELECT folder.LocalPath FROM filters LEFT JOIN folders ON filters.FolderID=folders.ID WHERE filters.ID=? AND filters.FilterType=?", aArgs );
    			if( !oResults.moveToFirst() )
    				throw new Exception("Invalid filter ID passed to getFilenameFilter");
    			
    			return new CloneFilenameFilter( oResults.getString( 0 ) );
    		}
    	}catch(SQLException e) {
    		e.printStackTrace();
    	}
		throw new Exception( "Invalid Filename Filter Type Selected");
	}
}
