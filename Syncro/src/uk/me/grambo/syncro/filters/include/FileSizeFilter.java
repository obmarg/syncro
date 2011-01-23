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
