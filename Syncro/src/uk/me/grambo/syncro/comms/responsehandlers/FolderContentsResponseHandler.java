package uk.me.grambo.syncro.comms.responsehandlers;

import java.io.IOException;
import java.io.InputStream;

import android.util.Log;
import uk.me.grambo.syncro.comms.pb.Folders;
import uk.me.grambo.syncro.comms.pb.Folders.FolderContents;

import uk.me.grambo.syncro.comms.FolderContentsHandler;
import uk.me.grambo.syncro.comms.PBSocketInterface;
import uk.me.grambo.syncro.comms.RemoteFileData;

public class FolderContentsResponseHandler implements PBResponseHandler {

	private FolderContentsHandler	m_dataHandler;
	private	int					 	m_folderId;
	
	public FolderContentsResponseHandler( 
			int folderId, 
			FolderContentsHandler handler 
			)
	{
		m_dataHandler = handler;
		m_folderId = folderId;
	}
	
	@Override
	public boolean canHandleResponse(int innType) 
	{
		if( innType == PBSocketInterface.ResponseTypes.FOLDER_CONTENTS_RESPONSE )
			return true;
		return false;
	}

	@Override
	public boolean canRemove() 
	{
		return true;
	}

	@Override
	public boolean handleResponse(
			int[] subpacketSizes,
			PBSocketInterface pbInterface
			) throws Exception, IOException 
	{
		if( subpacketSizes.length != 1 )
			throw new Exception("Invalid number of subpackets");
		FolderContents.Builder builder = FolderContents.newBuilder();
		pbInterface.ReadMessage( 
				builder,
				subpacketSizes[0]
				);
		FolderContents list = builder.build();
		processFolder( list, "" );
		
		return true;
	}
	
	private void processFolder( Folders.FolderContents folder, String path )
	{
		Log.d("Syncro", "Processing folder: " + folder.getName() );
		
		String newPath;
		if( path != "" )
			newPath = path + "/" + folder.getName();
		else
			newPath = folder.getName();
		Log.d( "Syncro", "new path: " + newPath );
		
		for(
				int fileNum=0;
				fileNum < folder.getFilesCount();
				fileNum++
				)
		{
			processFile( folder.getFiles( fileNum ), newPath );
		}
		for( 
				int folderNum=0; 
				folderNum < folder.getSubfoldersCount(); 
				folderNum++ 
		   		)
		{
			
			processFolder( 
					folder.getSubfolders( folderNum ),
					newPath
					);
		}
	}
	
	private void processFile( Folders.FileInfo file, String path )
	{
		RemoteFileData fileData = 
			new RemoteFileData(	file, m_folderId );
		if( path != "" )
		{
			fileData.Filename = path + "/" + fileData.Filename;
		}
		Log.d("Syncro", "Filename: " + fileData.Filename );
		//TODO: Move support for this to the RemoteFileData constructor sometime
		m_dataHandler.handleRemoteFile( fileData );
	}

}
