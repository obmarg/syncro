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

package uk.me.grambo.syncro.filters.filename;

import java.io.File;
import java.io.IOException;

import uk.me.grambo.syncro.FilenameFilter;
import uk.me.grambo.syncro.comms.RemoteFileData;

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
