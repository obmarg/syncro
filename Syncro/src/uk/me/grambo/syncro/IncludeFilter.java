package uk.me.grambo.syncro;

public interface IncludeFilter {
	public boolean shouldInclude(RemoteFileHandler.RemoteFileData inoFile);
	public boolean shouldInclude(RemoteFileHandler.RemoteFileData inoFile, String insDestinationFilename);
	public boolean shouldEndList();
	public boolean needsFilename();
}
