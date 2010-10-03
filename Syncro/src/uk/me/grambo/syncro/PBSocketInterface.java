package uk.me.grambo.syncro;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import com.google.protobuf.CodedInputStream;
import com.google.protobuf.ExtensionRegistryLite;
import com.google.protobuf.GeneratedMessageLite;
import uk.me.grambo.syncro.pb.Header;

public class PBSocketInterface {
	private static final byte PB_REQUEST_FIRST_BYTE = 105;
	private static final byte PB_RESPONSE_FIRST_BYTE = 106;
	
	private ArrayList<PBResponseHandler> m_aResponseHandlers;
	
	public class RequestTypes {
		public static final int BINARY_REQUEST = 1;
		public static final int BINARY_CONTINUE = 2;
	};
	
	public class ResponseTypes {
		public static final int BINARY_RESPONSE = 3;
	};

	public PBSocketInterface() {
		m_aResponseHandlers = new ArrayList<PBResponseHandler>();
	}
	
	public void WriteInitialHeader(OutputStream inoStream,int innHeaderSize) throws IOException {
		DataOutputStream oOutput = new DataOutputStream(inoStream);
		oOutput.writeByte(PB_REQUEST_FIRST_BYTE);
		oOutput.writeInt(innHeaderSize);
		oOutput.flush();
	}
	
	public void SendMessage(OutputStream inoStream,int inType) throws IOException {
		Header.PacketHeader oHeader = Header.PacketHeader.newBuilder()
				.setPacketType(inType)
				.build();
		WriteInitialHeader(inoStream,oHeader.getSerializedSize());
		oHeader.writeTo(inoStream);
		inoStream.flush();
	}
	
	public void SendObject(OutputStream inoStream,int inType,GeneratedMessageLite inoMessage) throws IOException {
		//Wraps the object in a header and sends it
		Header.PacketHeader oHeader = Header.PacketHeader.newBuilder()
				.setPacketType(inType)
				.addSubpacketSizes( inoMessage.getSerializedSize() )
				.build();
		WriteInitialHeader(inoStream,oHeader.getSerializedSize());
		oHeader.writeTo(inoStream);
		inoMessage.writeTo(inoStream);
		inoStream.flush();
	}
	
	public void HandleResponse(InputStream inoStream) throws Exception,IOException {
		DataInputStream oInput = new DataInputStream(inoStream);
		byte nFirstByte = oInput.readByte();
		if( nFirstByte != PB_RESPONSE_FIRST_BYTE )
			throw new Exception( "Invalid first byte in PBSocketInterface::GetResponse");
		int nHeaderSize = oInput.readInt();
		byte aBuffer[] = new byte[nHeaderSize];
		oInput.read(aBuffer);
		/*CodedInputStream oMessageInputStream = CodedInputStream.newInstance(inoStream);
		//oMessageInputStream.setSizeLimit(nHeaderSize); */
		Header.PacketHeader.Builder oHeaderBuilder = Header.PacketHeader.newBuilder();
		//oHeaderBuilder.mergeFrom(oMessageInputStream);
		//TODO: Figure out how to use CodedInputStream
		oHeaderBuilder.mergeFrom(aBuffer);
		PBResponseHandler oSelectedHandler = null;
		for( PBResponseHandler oHandler : m_aResponseHandlers ) {
			if( oHandler.canHandleResponse( oHeaderBuilder.getPacketType() ) ) {
				int nSubpacketSizes[] = new int[oHeaderBuilder.getSubpacketSizesCount()];
				for(int nSubpacket=0; nSubpacket < oHeaderBuilder.getSubpacketSizesCount(); nSubpacket++ ) {
					nSubpacketSizes[nSubpacket] = oHeaderBuilder.getSubpacketSizes(nSubpacket);
				}
				oHandler.handleResponse(nSubpacketSizes, inoStream);
				oSelectedHandler = oHandler;
				break;
			}
		}
		if( oSelectedHandler != null ) {
			if( oSelectedHandler.canRemove() )
				m_aResponseHandlers.remove(oSelectedHandler);
		} else {
			throw new Exception("Unknown packet type sent to PBSocketInterface::GetResponse");
		}
	}
	
	public void addResponseHandler(PBResponseHandler inoHandler) {
		m_aResponseHandlers.add(inoHandler);
	}
	
	public void clearAllResponseHandlers() {
		m_aResponseHandlers.clear();
	}
}
