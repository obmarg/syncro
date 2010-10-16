package uk.me.grambo.syncro.filters.include;

import uk.me.grambo.syncro.IncludeFilter;
import uk.me.grambo.syncro.RemoteFileHandler.RemoteFileData;

public class FileTimeFilter implements IncludeFilter {

	public FileTimeFilter() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public boolean shouldInclude(RemoteFileData inoFile,String insDestinationFilename) {
		//TODO: compare incoming and destination file times and return a value
		return true;
	}
	
	public boolean shouldInclude(RemoteFileData inoFile) {
		return false;
	}

	@Override
	public boolean shouldEndList() {
		// TODO Auto-generated method stub
		return false;
	}
	
	public boolean needsFilename() {
		return true;
	}

}
