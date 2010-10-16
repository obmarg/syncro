package uk.me.grambo.syncro;

import java.util.ArrayList;

import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import uk.me.grambo.syncro.filters.filename.*;
import uk.me.grambo.syncro.filters.include.AlwaysIncludeFilter;
import uk.me.grambo.syncro.filters.include.FileTimeFilter;

public class FilterFactory {

	protected Context m_oContext;
	
	public FilterFactory(Context inoContext) {
		m_oContext = inoContext;
	}
	
	private static final int FileTimeIncludeFilterId = 1;
	private static final int CloneFilenameFilterId = 2;
	private static final int AlwaysIncludeFilterId = 3;
	
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
			inoList.add( getIncludeFilter( FileTimeIncludeFilterId, 0 ) );
			inoList.add( getIncludeFilter( AlwaysIncludeFilterId, 0 ) );
		}
		oResults.close();
	}
	
	public void getFilenameFilters( SQLiteDatabase inoDB, int innFolderID, ArrayList<FilenameFilter> inoList ) throws Exception {
		String aArgs[] = { Integer.toString(innFolderID), Integer.toString(1) };
		Cursor oResults = inoDB.rawQuery("SELECT FilterType,ID FROM filters WHERE FolderId=? AND FilenameType=?", aArgs);
		if( oResults.moveToFirst() ) {
			do {
				FilenameFilter oItem = getFilenameFilter( oResults.getInt( 0 ), oResults.getInt( 1 ), inoDB ); 
				inoList.add( oItem );
			}while( oResults.moveToNext() );
		} else {
			//TODO: Remove this after the GUI filter management is finished
			String aArgs2[] = { Integer.toString(innFolderID) };
			Cursor oPathResults = inoDB.rawQuery("SELECT LocalPath FROM folders WHERE ID=?", aArgs2 );
			if( oPathResults.moveToFirst() ) {
				inoList.add( new CloneFilenameFilter( oPathResults.getString(0) ) );
			} else {
				throw new Exception("could not find local path for folder in getFilenameFilters");
			}
			oPathResults.close();
		}
		oResults.close();
	}

	public IncludeFilter getIncludeFilter( int nFilterType, int nFilterId ) throws Exception {
		if( nFilterType == FileTimeIncludeFilterId )
			return new FileTimeFilter();
		else if( nFilterType == AlwaysIncludeFilterId )
			return new AlwaysIncludeFilter();
		throw new Exception( "Invalid Include Filter Type Selected");
	}
	
	
	public FilenameFilter getFilenameFilter( int innFilterType, int innFilterId, SQLiteDatabase inoDB ) throws Exception {
		FilenameFilter oRV;
		if( innFilterType == CloneFilenameFilterId ) {
			String aArgs[] = { Integer.toString( innFilterId ), Integer.toString( innFilterType ) };
    		Cursor oResults = inoDB.rawQuery("SELECT folders.LocalPath FROM filters LEFT JOIN folders ON filters.FolderID=folders.ID WHERE filters.ID=? AND filters.FilterType=?", aArgs );
    		if( !oResults.moveToFirst() ) {
    			throw new Exception("Invalid filter ID passed to getFilenameFilter");
    		}
    		
    		String sLocalPath = oResults.getString( 0 );
    		oResults.close();
    		
    		oRV = new CloneFilenameFilter( sLocalPath );
    	} else
    		throw new Exception( "Invalid Filename Filter Type Selected");
		return oRV;
	}
}
