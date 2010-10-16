package uk.me.grambo.syncro.filters.include;

import uk.me.grambo.syncro.IncludeFilter;
import uk.me.grambo.syncro.RemoteFileHandler.RemoteFileData;

public class AlwaysIncludeFilter implements IncludeFilter {

	@Override
	public boolean shouldInclude(RemoteFileData inoFile) {
		return true;
	}

	@Override
	public boolean shouldInclude(RemoteFileData inoFile,
			String insDestinationFilename) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean shouldEndList() {
		return true;
	}

	@Override
	public boolean needsFilename() {
		return false;
	}

}
