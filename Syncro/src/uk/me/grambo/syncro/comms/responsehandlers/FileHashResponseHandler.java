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

package uk.me.grambo.syncro.comms.responsehandlers;

import java.io.IOException;

import uk.me.grambo.syncro.comms.PBSocketInterface;
import uk.me.grambo.syncro.comms.pb.Binarydata;

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
	public boolean handleResponse(
			int[] subpacketSizes,
			PBSocketInterface pbInterface
			) throws Exception, IOException 
	{
		if( subpacketSizes.length != 1 )
			throw new Exception( "Invalid number of subpackets" );
		
		Binarydata.FileHashResponse.Builder message = 
			Binarydata.FileHashResponse.newBuilder();
		pbInterface.ReadMessage(
				message,
				subpacketSizes[0]
				);
		
		if( message.hasOk() )
			m_hashOk = message.getOk();
		
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
