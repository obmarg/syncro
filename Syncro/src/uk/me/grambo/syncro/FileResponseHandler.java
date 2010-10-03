package uk.me.grambo.syncro;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import uk.me.grambo.syncro.pb.Binarydata;

import com.google.protobuf.CodedInputStream;
import com.google.protobuf.ExtensionRegistryLite;

public class FileResponseHandler implements PBResponseHandler {
	private OutputStream m_oDest;
	private boolean m_fDone;

	public FileResponseHandler(OutputStream inoDestination) {
		m_oDest = inoDestination;
		m_fDone = false;
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
		inoStream.read(aBuffer);
		//TODO: figure out how to use CodedInputStream
		//CodedInputStream oHeaderStream = CodedInputStream.newInstance(inoStream);
		//oHeaderStream.setSizeLimit(nSubpacketSizes[0]);
		
		//oPacketInfo.mergeFrom(oHeaderStream);
		Binarydata.BinaryPacketHeader.Builder oPacketInfo = Binarydata.BinaryPacketHeader.newBuilder();
		oPacketInfo.mergeFrom(aBuffer);
		
		Binarydata.BinaryPacketHeader.SectionType eSection = oPacketInfo.getBinaryPacketType();
		if( eSection == Binarydata.BinaryPacketHeader.SectionType.END )
			m_fDone = true;
		byte nPacketContents[] = new byte[ nSubpacketSizes[1] ];
		//TODO: add some sort of size check on this?
		inoStream.read(nPacketContents);
		m_oDest.write(nPacketContents);
		
		return true;
	}

	@Override
	public boolean canRemove() {
		return m_fDone;
	}

}
