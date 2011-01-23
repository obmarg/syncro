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

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.util.Log;
import uk.me.grambo.syncro.pb.Handshake;
import uk.me.grambo.syncro.responsehandlers.PBResponseHandler;

public class HandshakeHandler implements PBResponseHandler {
	
	private static final String MAGIC_REQUEST_STRING = "Hello Syncro?";
	private static final String MAGIC_RESPONSE_STRING = "Hey bitch!";
	
	private boolean m_fHandshakeDone;

	public HandshakeHandler() {
		// TODO Auto-generated constructor stub
		m_fHandshakeDone = false;
	}

	@Override
	public boolean canHandleResponse(int innType) {
		if( innType == PBSocketInterface.ResponseTypes.HANDSHAKE_RESPONSE )
			return true;
		return false;
	}

	@Override
	public boolean handleResponse(int[] nSubpacketSizes, InputStream inoStream)
			throws Exception, IOException {
		if( nSubpacketSizes.length != 1 )
			throw new Exception("Invalid number of subpackets sent in handshake response");
		
		byte[] aBuffer = new byte[ nSubpacketSizes[0] ];
		Log.d("Syncro", "Reading first subpacket: " + nSubpacketSizes[0]);
		inoStream.read(aBuffer);
		//TODO: figure out how to use CodedInputStream
		
		
		Handshake.HandshakeResponse.Builder oResponse = Handshake.HandshakeResponse.newBuilder();
		oResponse.mergeFrom(aBuffer);
		if( ( !oResponse.hasMagic() ) || (oResponse.getMagic().compareTo(MAGIC_RESPONSE_STRING) != 0) ) {
			throw new Exception("Invalid magic!");
		}
		//TODO: add uuid checking stuff in here
		m_fHandshakeDone = true;
		return true;
	}

	@Override
	public boolean canRemove() {
		return true;
	}
	
	public void writeRequest(PBSocketInterface inoInterface,OutputStream inoStream) throws IOException {
		Handshake.HandshakeRequest oMessage = Handshake.HandshakeRequest.newBuilder()
			.setClientVerMajor(0)
			.setClientVerMinor(0)
			.setMagic(MAGIC_REQUEST_STRING)
			.build();
		inoInterface.SendObject(inoStream, PBSocketInterface.RequestTypes.HANDSHAKE_REQUEST, oMessage);
	}
	
	public boolean getHandshakeDone() {
		return m_fHandshakeDone;
	}

}
