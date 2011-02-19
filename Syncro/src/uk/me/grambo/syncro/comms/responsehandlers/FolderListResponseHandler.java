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

package uk.me.grambo.syncro.comms.responsehandlers;

import java.io.IOException;
import java.io.InputStream;

import android.database.sqlite.SQLiteStatement;

import uk.me.grambo.syncro.comms.FolderListHandler;
import uk.me.grambo.syncro.comms.PBSocketInterface;
import uk.me.grambo.syncro.comms.pb.Folders;
import uk.me.grambo.syncro.comms.pb.Folders.FolderList;

public class FolderListResponseHandler implements PBResponseHandler {
	
	private FolderListHandler m_dataHandler;

	public FolderListResponseHandler(FolderListHandler dataHandler)
	{
		m_dataHandler = dataHandler;
	}
	
	@Override
	public boolean canHandleResponse(int innType) 
	{
		if( innType == PBSocketInterface.ResponseTypes.FOLDER_LIST_RESPONSE )
			return true;
		return false;
	}

	@Override
	public boolean handleResponse(
			int[] subpacketSizes, 
			PBSocketInterface pbInterface
			) throws Exception, IOException 
	{
		if( subpacketSizes.length != 1 )
			throw new Exception("Invalid number of subpackets");
		FolderList.Builder builder = FolderList.newBuilder();
		pbInterface.ReadMessage( 
				builder,
				subpacketSizes[0]
				);
		FolderList list = builder.build();
		for( int folderNum = 0; folderNum < list.getFoldersCount(); folderNum++ )
		{ 
			m_dataHandler.handlerFolder( 
					new FolderListHandler.FolderInfo( 
							list.getFolders( folderNum ) 
							) 
					);
		}
		
		return true;
	}

	@Override
	public boolean canRemove() 
	{
		return true;
	}

}
