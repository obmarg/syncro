package uk.me.grambo.syncro.filters.include;

import java.io.File;

import uk.me.grambo.syncro.IncludeFilter;
import uk.me.grambo.syncro.RemoteFileHandler.RemoteFileData;

public class FileSizeFilter implements IncludeFilter {

	public FileSizeFilter() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public boolean shouldInclude(RemoteFileData inoFile,String insDestinationFilename) {
		File oFile = new File( insDestinationFilename );
		if( oFile.length() != inoFile.Size )
			return true;
		return false;
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
