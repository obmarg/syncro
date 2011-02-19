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

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

import android.util.Log;

import com.google.protobuf.InvalidProtocolBufferException;

import uk.me.grambo.syncro.comms.PBSocketInterface;
import uk.me.grambo.syncro.comms.PBSocketInterface.ResponseTypes;
import uk.me.grambo.syncro.comms.pb.Binarydata;

public class UploadResponseHandler implements PBResponseHandler {
	
	private int m_lastType;
	private byte[] m_buffer; 
	private int m_maxPacketSize;
	private boolean m_finished;
	private long m_sizeOnServer;
	
	public UploadResponseHandler()
	{
		m_finished = false;
		m_sizeOnServer = 0;
	}

	@Override
	public boolean canHandleResponse(int innType) {
		if( 
				(innType == PBSocketInterface.ResponseTypes.BINARY_INCOMING_RESPONSE)
				|| ( innType == PBSocketInterface.ResponseTypes.BINARY_INCOMING_DATA_ACK )
				)
		{
			m_lastType = innType;
			return true;
		}
		return false;
	}
	
	@Override
	public boolean handleResponse(
			int[] subpacketSizes,
			PBSocketInterface pbInterface
			) throws Exception, IOException 
	{
		switch( m_lastType )
		{
		case PBSocketInterface.ResponseTypes.BINARY_INCOMING_RESPONSE:
			return handleIncomingResponse( subpacketSizes, pbInterface );
		case PBSocketInterface.ResponseTypes.BINARY_INCOMING_DATA_ACK:
			return handleIncomingDataAck( subpacketSizes, pbInterface );
		}
		return false;
	}
	
	@Override
	public boolean canRemove() {
		// TODO Auto-generated method stub
		return m_finished;
	}

	public boolean handleIncomingResponse(
			int[] subpacketSizes, 
			PBSocketInterface pbInterface
			) throws Exception
	{
		if( subpacketSizes.length != 1 )
		{
			throw new Exception("Invalid number of subpackets sent in binary incoming response packet");
		}
		
		Binarydata.BinaryIncomingResponse.Builder response = 
			Binarydata.BinaryIncomingResponse.newBuilder();
		pbInterface.ReadMessage( response, subpacketSizes[0] );
		if( response.getAccepted() )
		{
			m_maxPacketSize = response.getMaxPacketSize();
			if( response.hasCurrentFileSize() )
			{
				m_sizeOnServer = response.getCurrentFileSize();
			}
			//Log.d("Syncro", "Incoming Response: Accepted");
			//Log.d("Syncro", "Packet Size: " + m_maxPacketSize);
			return true;
		} 
		else
		{
			//Log.d("Syncro", "Incoming Response: Not Accepted");
		}
		m_finished = true;
		return false;
	}

	public boolean handleIncomingDataAck(
			int[] subpacketSizes,
			PBSocketInterface pbInterface
			) throws Exception
	{
		if( subpacketSizes.length != 1 )
		{
			throw new Exception("Invalid number of subpackets sent in binary incoming response packet");
		}
		
		Binarydata.BinaryIncomingAck.Builder response = 
			Binarydata.BinaryIncomingAck.newBuilder(); 
		pbInterface.ReadMessage( response, subpacketSizes[0] );
		
		if( response.getOk() )
		{
			return true;
		}
		m_finished = true;
		return false;
	}

	public int getMaxPacketSize() {
		return m_maxPacketSize;
	}
	public long getSizeOnServer() {
		return m_sizeOnServer;
	}
}
