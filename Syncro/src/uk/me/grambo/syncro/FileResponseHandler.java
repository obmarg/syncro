package uk.me.grambo.syncro;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import uk.me.grambo.syncro.pb.Binarydata;

import android.util.Log;

import com.google.protobuf.CodedInputStream;
import com.google.protobuf.ExtensionRegistryLite;

public class FileResponseHandler implements PBResponseHandler {
	private OutputStream m_oDest;
	private boolean m_fDone;
	private byte[] m_aRecvBuffer;
	private int m_nRecievedBytes;

	public FileResponseHandler(OutputStream inoDestination) {
		m_oDest = inoDestination;
		m_fDone = false;
		m_nRecievedBytes = 0;
	}

	@Override
	public boolean canHandleResponse(int innType) {
		if( innType == PBSocketInterface.ResponseTypes.BINARY_RESPONSE )
			return true;
		return false;
	}

	@Override
	public boolean handleResponse(int[] nSubpacketSizes, InputStream inoStream) throws Exception,IOException {
		if( nSubpacketSizes.length != 2 ) {
			//Should always be 2 subpackets for binary data
			throw new Exception("Invalid number of subpackets sent in Binary Response packet");
		}
		byte[] aBuffer = new byte[ nSubpacketSizes[0] ];
		Log.d("Syncro", "Reading first subpacket: " + nSubpacketSizes[0]);
		inoStream.read(aBuffer);
		//TODO: figure out how to use CodedInputStream
		//CodedInputStream oHeaderStream = CodedInputStream.newInstance(inoStream);
		//oHeaderStream.setSizeLimit(nSubpacketSizes[0]);
		
		//oPacketInfo.mergeFrom(oHeaderStream);
		Binarydata.BinaryPacketHeader.Builder oPacketInfo = Binarydata.BinaryPacketHeader.newBuilder();
		oPacketInfo.mergeFrom(aBuffer);
		Log.d("Syncro","Recieved Binary Header:\n" + oPacketInfo.toString() );
		
		Binarydata.BinaryPacketHeader.SectionType eSection = oPacketInfo.getBinaryPacketType();
		if( eSection == Binarydata.BinaryPacketHeader.SectionType.END )
			m_fDone = true;
		if( (m_aRecvBuffer == null) || (m_aRecvBuffer.length < nSubpacketSizes[1])) {
			m_aRecvBuffer = new byte[ nSubpacketSizes[1] ];
		}
		//TODO: Ensure we have read enough but not too much
		int nSizeRead = 0;
		do {
			int nLeftToRead = nSubpacketSizes[1] - nSizeRead;
			nSizeRead = nSizeRead + inoStream.read( m_aRecvBuffer, nSizeRead, nLeftToRead );
			//TODO: Maybe write out the data in this loop as well
			//		probably best to do some investigation to see which gets better performance?
		}while( (nSizeRead < nSubpacketSizes[1]) );
		Log.d("Syncro", "Finished reading: " + nSizeRead + " bytes.  Expected: " +nSubpacketSizes[1] + "\n");
		m_oDest.write(m_aRecvBuffer,0, nSubpacketSizes[1]);
		m_nRecievedBytes += nSubpacketSizes[1];
		
		return true;
	}

	@Override
	public boolean canRemove() {
		return m_fDone;
	}
	
	public int getRecievedSize() {
		return m_nRecievedBytes;
	}

}
