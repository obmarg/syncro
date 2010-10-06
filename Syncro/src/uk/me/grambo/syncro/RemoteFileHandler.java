package uk.me.grambo.syncro;

public interface RemoteFileHandler {
	public class RemoteFileData {
		public String Filename;
		public int FolderId;
		public int Size;
	}
	public void HandleRemoteFile(RemoteFileData inoFile);
}
