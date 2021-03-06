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


package uk.me.grambo.syncro.comms;

import uk.me.grambo.syncro.comms.pb.Folders;

/**
 * An interface used to receive folder list information
 * @author Grambo <grambo@grambo.me.uk>
 *
 */
public interface FolderListHandler {
	void handleFolder( FolderInfo folder );
	
	public class FolderInfo {
		public int			Id;
		public String		Name;
		public String		Path;
		public boolean		Writable;
		public boolean		Readable;
		
		public FolderInfo() {};
		public FolderInfo( Folders.FolderInfo other )
		{
			Id 			= other.getFolderId();
			Name 		= other.getFolderName();
			Path		= other.getFolderPath();
			Writable 	= other.getCanWrite();
			Readable	= other.getCanRead();
		}
	}
};
