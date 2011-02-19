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
import java.io.InputStream;
import java.io.OutputStream;

import uk.me.grambo.syncro.comms.PBSocketInterface;
import uk.me.grambo.syncro.comms.PBSocketInterface.ResponseTypes;
import uk.me.grambo.syncro.comms.pb.Binarydata;

import android.util.Log;

import com.google.protobuf.CodedInputStream;
import com.google.protobuf.ExtensionRegistryLite;

public class DownloadResponseHandler implements PBResponseHandler {
	private OutputStream m_destStream;
	private boolean m_done;
	private byte[] m_recvBuffer;
	private int m_recievedBytes;
	private boolean m_rejected;

	public DownloadResponseHandler(OutputStream destinationStream) {
		m_destStream = destinationStream;
		m_done = false;
		m_recievedBytes = 0;
		m_rejected = false;
	}

	@Override
	public boolean canHandleResponse(int innType) {
		if( innType == PBSocketInterface.ResponseTypes.BINARY_REQUEST_REJECTED )
		{
			m_rejected = true;
			return true;
		}
		if( innType == PBSocketInterface.ResponseTypes.BINARY_RESPONSE )
			return true;
		return false;
	}
	
	@Override
	public boolean handleResponse(
			int[] subpacketSizes,
			PBSocketInterface pbInterface
			) throws Exception, IOException 
	{
		if( m_rejected )
			return true;
		if( subpacketSizes.length != 2 ) {
			//Should always be 2 subpackets for binary data
			throw new Exception("Invalid number of subpackets sent in Binary Response packet");
		}
		
		Binarydata.BinaryPacketHeader.Builder oPacketInfo = 
			Binarydata.BinaryPacketHeader.newBuilder();
		pbInterface.ReadMessage( oPacketInfo, subpacketSizes[0] );
		
		Binarydata.BinaryPacketHeader.SectionType eSection = oPacketInfo.getBinaryPacketType();
		if( eSection == Binarydata.BinaryPacketHeader.SectionType.END )
			m_done = true;
		
		if( (m_recvBuffer == null) || (m_recvBuffer.length < subpacketSizes[1])) {
			m_recvBuffer = new byte[ subpacketSizes[1] ];
		}
		pbInterface.ReadData( m_recvBuffer, subpacketSizes[1] );
		m_destStream.write(m_recvBuffer,0, subpacketSizes[1]);
		m_recievedBytes += subpacketSizes[1];
		
		return false;
	}

	@Override
	public boolean canRemove() {
		return m_done || m_rejected;
	}
	
	public int getRecievedSize() {
		return m_recievedBytes;
	}
	
	/**
	 * 
	 * @return Returns true if this file download request was rejected
	 */
	public boolean getRejected()
	{
		return m_rejected;
	}
}
