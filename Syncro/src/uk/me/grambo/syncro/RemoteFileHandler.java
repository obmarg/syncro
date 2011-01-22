package uk.me.grambo.syncro;

public interface RemoteFileHandler {
	public class RemoteFileData {
		public String Filename;
		public int FolderId;
		public long Size;
	}
	public void HandleRemoteFile(RemoteFileData inoFile);
}
