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
 * Class to store information on each remote file
 * @author Grambo <grambo@grambo.me.uk>
 *
 */
public class RemoteFileData {
	public String Filename;
	public int FolderId;
	public long Size;
	public RemoteFileData() {};
	public RemoteFileData( Folders.FileInfo info, int folderId )
	{
		Filename = 	info.getName();
		FolderId = 	folderId;
		Size	 = 	info.getSize();
	}
}