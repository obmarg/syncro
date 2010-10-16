package uk.me.grambo.syncro;

public interface FilenameFilter {
	public boolean canHandle(RemoteFileHandler.RemoteFileData inoFile);
	public String getDestinationFilename(RemoteFileHandler.RemoteFileData inoFile);
}
