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

package uk.me.grambo.syncro.responsehandlers;

import java.io.IOException;
import java.io.InputStream;

import android.database.sqlite.SQLiteStatement;

import uk.me.grambo.syncro.PBSocketInterface;
import uk.me.grambo.syncro.pb.Folders;
import uk.me.grambo.syncro.pb.Folders.FolderList;

public class FolderListResponseHandler implements PBResponseHandler {
	
	private SQLiteStatement m_insertStatement;

	public FolderListResponseHandler(SQLiteStatement insertStatement)
	{
		m_insertStatement = insertStatement;
	}
	
	@Override
	public boolean canHandleResponse(int innType) 
	{
		if( innType == PBSocketInterface.ResponseTypes.FOLDER_LIST_RESPONSE )
			return true;
		return false;
	}

	@Override
	public boolean handleResponse(int[] nSubpacketSizes, InputStream inoStream)
			throws Exception, IOException 
	{
		if( nSubpacketSizes.length != 1 )
			throw new Exception("Invalid number of subpackets");
		byte[] buffer = new byte[ nSubpacketSizes[0] ];
		int read = 0;
		do
		{
			read += inoStream.read(buffer, read, buffer.length - read );
		}while( read != buffer.length );
		FolderList list = FolderList.parseFrom( buffer );
		for( int folderNum = 0; folderNum < list.getFoldersCount(); folderNum++ )
		{
			Folders.FolderInfo folder = list.getFolders( folderNum );
			m_insertStatement.bindLong( 1, folder.getFolderId() );
			m_insertStatement.bindString( 2,  folder.getFolderName() );
			m_insertStatement.bindString( 3,  folder.getFolderPath() );
			m_insertStatement.executeInsert();
			//TODO: Should probably add some pruning logic etc. in here
			//		for folders no longer on the server
		}
		
		return true;
	}

	@Override
	public boolean canRemove() 
	{
		return true;
	}

}
