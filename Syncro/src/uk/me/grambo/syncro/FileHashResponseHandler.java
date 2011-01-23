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

package uk.me.grambo.syncro;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

import uk.me.grambo.syncro.pb.Binarydata;

public class FileHashResponseHandler implements PBResponseHandler { 
	
	public FileHashResponseHandler()
	{
		m_hashOk = false;
	}
	
	@Override
	public boolean canHandleResponse(int innType) {
		if( innType == PBSocketInterface.ResponseTypes.FILE_HASH_RESPONSE )
			return true;
		return false;
	}

	@Override
	public boolean handleResponse(int[] nSubpacketSizes, InputStream inoStream)
			throws Exception, IOException {
		DataInputStream input = new DataInputStream( inoStream );
		if( nSubpacketSizes.length != 1 )
			throw new Exception( "Invalid number of subpackets" );
		byte[] aBuffer = new byte[ nSubpacketSizes[0] ];
		
		input.readFully(aBuffer);
		Binarydata.FileHashResponse.Builder oBuilder = Binarydata.FileHashResponse.newBuilder();
		oBuilder.mergeFrom(aBuffer);
		if( oBuilder.hasOk() )
			m_hashOk = oBuilder.getOk();
		
		return true;
	}

	@Override
	public boolean canRemove() {		
		return true;
	}
	
	public boolean hashOk()
	{
		return m_hashOk;
	}

	private boolean m_hashOk;
}
