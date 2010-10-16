package uk.me.grambo.syncro.filters.filename;

import java.io.File;
import java.io.IOException;

import uk.me.grambo.syncro.FilenameFilter;
import uk.me.grambo.syncro.RemoteFileHandler.RemoteFileData;

public class CloneFilenameFilter implements FilenameFilter {

	protected String m_sLocalPath;
	
	public CloneFilenameFilter(String insLocalPath) {
		m_sLocalPath = insLocalPath;
		if( !m_sLocalPath.endsWith( "/" ) )
			m_sLocalPath = m_sLocalPath + "/";
	}
	
	@Override
	public boolean canHandle(RemoteFileData inoFile) {
		return true;
	}

	@Override
	public String getDestinationFilename(RemoteFileData inoFile) {
		try {
			if( m_sLocalPath == null )
				throw new Exception("Local path is null in getDestinationFilename");		//TODO: Fix this to throw exception, just being really really lazy
			String insDestinationFolder = m_sLocalPath;
			//TODO: add code to get this from the database etc.
			//TODO: check for ..s or something?
			File oFile = new File( insDestinationFolder + inoFile.Filename );
			oFile.getParentFile().mkdirs();
			String rv = oFile.getCanonicalPath();
			return rv;
		}catch(Exception e) {
			e.printStackTrace();
		}
		//TODO: make this throw exception or something at some point
		return "";
	}

}
