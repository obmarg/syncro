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

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import android.util.Log;

import com.google.protobuf.CodedInputStream;
import com.google.protobuf.ExtensionRegistryLite;
import com.google.protobuf.GeneratedMessageLite;
import com.google.protobuf.MessageLite;

import uk.me.grambo.syncro.comms.pb.Header;
import uk.me.grambo.syncro.comms.pb.Folders.FolderList;
import uk.me.grambo.syncro.comms.responsehandlers.PBResponseHandler;

/**
 * Class responsible for reading & writing protocol buffers to/from streams
 * @author Grambo <grambo@grambo.me.uk>
 *
 */
public class PBSocketInterface {
	private static final byte PB_REQUEST_FIRST_BYTE = 105;
	private static final byte PB_RESPONSE_FIRST_BYTE = 106;
	
	private static final int	DEFAULT_RECV_BUFFER_SIZE = 1024;
	private static final int	MAX_RECV_BUFFER_SIZE	 = 1024 * 1024;
	
	private byte[]	m_recvBuffer;
	
	private ArrayList<PBResponseHandler> m_aResponseHandlers;
	
	/**
	 * This stream should only be used during response handler calls
	 */
	private InputStream	m_tempStream;
	
	/**
	 * Class to hold the various RequestType ids
	 * @author Grambo <grambo@grambo.me.uk>
	 *
	 */
	public static class RequestTypes {
		public static final int BINARY_REQUEST 			= 1;
		public static final int BINARY_CONTINUE 		= 2;
		public static final int HANDSHAKE_REQUEST 		= 4;
		public static final int BINARY_INCOMING_REQUEST = 6;
		public static final int BINARY_INCOMING_DATA 	= 8;
		public static final int FOLDER_LIST_REQUEST		= 12;
		public static final int FILE_HASH_REQUEST 		= 16;
		public static final int FOLDER_CONTENTS_REQUEST = 18;
		public static String Str(int Type)
		{
			switch(Type)
			{
			case BINARY_REQUEST:
				return "Binary Request";
			case BINARY_CONTINUE:
				return "Binary Continue";
			case HANDSHAKE_REQUEST:
				return "Handshake Request";
			case BINARY_INCOMING_REQUEST:
				return "Binary Incoming Request";
			case BINARY_INCOMING_DATA:
				return "Binary Incoming Data";
			case FOLDER_LIST_REQUEST:
				return "Folder List Request";
			case FILE_HASH_REQUEST:
				return "File Hash Request";
			case FOLDER_CONTENTS_REQUEST:
				return "Folder Contents Request";
			}
			return "Unknown";
		}
	};
	
	/**
	 * Class to hold the various ResponseType ids
	 * @author Grambo <grambo@grambo.me.uk>
	 *
	 */
	public static class ResponseTypes {
		public static final int BINARY_RESPONSE 			= 3;
		public static final int HANDSHAKE_RESPONSE 			= 5;
		public static final int BINARY_INCOMING_RESPONSE 	= 7;
		public static final int BINARY_INCOMING_DATA_ACK 	= 9;
		public static final int FOLDER_LIST_RESPONSE		= 13;
		public static final int FILE_HASH_RESPONSE 			= 17;
		public static final int FOLDER_CONTENTS_RESPONSE 	= 19;
		public static final int BINARY_REQUEST_REJECTED		= 20;
		public static String Str(int Type)
		{
			switch(Type)
			{
			case BINARY_RESPONSE:
				return "Binary Resposne";
			case HANDSHAKE_RESPONSE:
				return "Handshake Response";
			case BINARY_INCOMING_RESPONSE:
				return "Binary Incoming Response";
			case BINARY_INCOMING_DATA_ACK:
				return "Binary Incoming Data Ack";
			case FOLDER_LIST_RESPONSE:
				return "Folder List Response";
			case FILE_HASH_RESPONSE:
				return "File Hash Response";
			case FOLDER_CONTENTS_RESPONSE:
				return "Folder Contents Response";
			case BINARY_REQUEST_REJECTED:
				return "Binary Request Rejected";
			}
			return "Unknown";
		}
	};

	public PBSocketInterface() {
		m_aResponseHandlers = new ArrayList<PBResponseHandler>();
		setupRecvBuffer();
	}
	
	/**
	 * Sends a basic pb message without any subpackets
	 * @param stream		Stream to write to
	 * @param requestType 	The type id of the message we are sending
	 * @throws IOException
	 */
	public void SendMessage( OutputStream stream,int requestType ) throws IOException {
		Header.PacketHeader oHeader = Header.PacketHeader.newBuilder()
				.setPacketType( requestType )
				.build();
		//Log.d("Syncro","Sending Message.  Header:\n" + oHeader.toString() );
		//Log.d("Syncro", "Sending message: " + RequestTypes.Str(inType));
		WriteInitialHeader( stream, oHeader.getSerializedSize() );
		oHeader.writeTo(stream);
		stream.flush();
		//Log.d("Syncro","Finished sending");
	}
	
	/**
	 * Sends a single protocol buffer message
	 * 
	 * @param stream		The stream to write to
	 * @param requestType	The requestType of the message we are sending
	 * @param message		The message to send
	 * @throws IOException
	 */
	public void SendObject( 
			OutputStream stream,
			int requestType,
			GeneratedMessageLite message
			) throws IOException 
	{
		//Wraps the object in a header and sends it
		Header.PacketHeader oHeader = Header.PacketHeader.newBuilder()
				.setPacketType(requestType)
				.addSubpacketSizes( message.getSerializedSize() )
				.build();
		//Log.d("Syncro","Sending Object.  Type: " + inType);
		//Log.d("Syncro","Header:\n" + oHeader.toString() );
		//Log.d("Syncro","Object:\n" + inoMessage.toString() );
		//Log.d("Syncro", "Sending message: " + RequestTypes.Str(inType));
		WriteInitialHeader(stream,oHeader.getSerializedSize());
		oHeader.writeTo(stream);
		message.writeTo(stream);
		stream.flush();
		//Log.d("Syncro","Finished sending");
	}
	
	/**
	 * Sends a protocol buffer message, followed by another subpacket of data
	 * 
	 * @param stream		The stream to write to 
	 * @param requestType	The request type of the message we are sending
	 * @param message		The message to send
	 * @param data			A buffer containing the data to send as the secondary
	 * 						subpacket
	 * @param dataSize		The size of the buffer above to send
	 * @throws IOException
	 */
	public void SendObjectAndData( 
			OutputStream stream, 
			int requestType, 
			GeneratedMessageLite message, 
			byte[] data, 
			int dataSize
			) throws IOException
	{
		Header.PacketHeader oHeader = Header.PacketHeader.newBuilder()
			.setPacketType(requestType)
			.addSubpacketSizes( message.getSerializedSize() )
			.addSubpacketSizes( dataSize )
			.build();
		
		/*Log.d("Syncro", 
				"Sending message: " + 
				RequestTypes.Str(inType) + 
				" (+" +
				Integer.valueOf(inDataSize).toString() + 
				" bytes)"
				); */
		WriteInitialHeader( stream, oHeader.getSerializedSize() );
		oHeader.writeTo( stream );
		message.writeTo( stream );
		stream.write( data, 0, dataSize );
		stream.flush();
		//Log.d("Syncro","Finished sending");
	}
	
	/**
	 * Reads a message from the temporary stream.
	 * Should only be used by PBResponseHandlers
	 * @param dest		A builder to build the message with
	 * @param size		The size to be read
	 * @throws Exception
	 * @throws IOException
	 */
	public void ReadMessage( MessageLite.Builder dest, int size ) throws Exception, IOException {
		if( m_tempStream == null )
		{
			throw new Exception( 
					"Stream == null in PBSocketInterface::ReadMessage" 
					);
		}
		ReadMessage( m_tempStream, dest, size );
	}
	
	/**
	 * Reads a PB message from the given input stream
	 * @param stream	The input stream to read a message from
	 * @param dest		A builder to build the message with
	 * @param size		The size to be read
	 * @throws Exception
	 * @throws IOException
	 */
	public void ReadMessage(InputStream stream, MessageLite.Builder dest, int size) 
	throws Exception, IOException 
	{
		setupRecvBuffer( size );
		int read = 0;
		do
		{
			read += stream.read( m_recvBuffer, read, size - read );
		}while( read != size );
		dest.mergeFrom( m_recvBuffer, 0, size );
	}
	
	/**
	 * Reads data from the temporary input stream into a buffer
	 * @param buffer
	 * @param size
	 * @throws Exception
	 * @throws IOException
	 */
	public void ReadData( byte[] buffer, int size ) throws Exception, IOException
	{
		if( m_tempStream == null )
		{
			throw new Exception( 
					"Stream == null in PBSocketInterface::ReadData" 
					);
		}
		ReadData( m_tempStream, buffer, size );
	}
	
	/**
	 * Reads data from an input stream into a buffer
	 * @param stream		The stream to read from
	 * @param buffer		The buffer to fill
	 * @param size			The amount of data to read
	 * @throws IOException 
	 */
	public void ReadData( InputStream stream, byte[] buffer, int size ) throws IOException
	{
		int read = 0;
		do
		{
			read += stream.read( buffer, read, size - read );
		}while( read != size );
	}
	
	/**
	 * Reads a single protocol buffer message from the stream, 
	 * and tries to use any available PBResponseHandlers
	 * to handle it
	 * 
	 * @param inoStream		The stream to read from
	 * @throws Exception
	 * @throws IOException
	 */
	public void HandleResponse(InputStream inoStream) throws Exception,IOException {
		try {
			DataInputStream oInput = new DataInputStream(inoStream);
			byte nFirstByte = oInput.readByte();
			if( nFirstByte != PB_RESPONSE_FIRST_BYTE )
				throw new Exception( "Invalid first byte in PBSocketInterface::GetResponse - " + nFirstByte);
			int nHeaderSize = oInput.readInt();
			//Log.d("Syncro","Reading Header Size: " + nHeaderSize+"\n");
			byte aBuffer[] = new byte[nHeaderSize];
			oInput.read(aBuffer);
			/*CodedInputStream oMessageInputStream = CodedInputStream.newInstance(inoStream);
			//oMessageInputStream.setSizeLimit(nHeaderSize); */
			Header.PacketHeader.Builder oHeaderBuilder = Header.PacketHeader.newBuilder();
			//oHeaderBuilder.mergeFrom(oMessageInputStream);
			//TODO: Figure out how to use CodedInputStream
			oHeaderBuilder.mergeFrom(aBuffer);
			//Log.d("Syncro","Received " + ResponseTypes.Str( oHeaderBuilder.getPacketType() ) );
			//Log.d("Syncro","Recieved Header:\n" + oHeaderBuilder.toString() );
			PBResponseHandler oSelectedHandler = null;
			for( PBResponseHandler oHandler : m_aResponseHandlers ) {
				if( oHandler.canHandleResponse( oHeaderBuilder.getPacketType() ) ) {
					int nSubpacketSizes[] = new int[oHeaderBuilder.getSubpacketSizesCount()];
					for(int nSubpacket=0; nSubpacket < oHeaderBuilder.getSubpacketSizesCount(); nSubpacket++ ) {
						nSubpacketSizes[nSubpacket] = oHeaderBuilder.getSubpacketSizes(nSubpacket);
					}
					m_tempStream = inoStream;
					oHandler.handleResponse(nSubpacketSizes, this);
					m_tempStream = null;
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
		finally
		{
			m_tempStream = null;
		}
	}
	
	public void addResponseHandler(PBResponseHandler inoHandler) {
		m_aResponseHandlers.add(inoHandler);
	}
	
	public void removeResponseHandler(PBResponseHandler inoHandler) {
		m_aResponseHandlers.remove(inoHandler);
	}
	
	public void clearAllResponseHandlers() {
		m_aResponseHandlers.clear();
	}
	
	/**
	 * Writes the initial protocol buffer header bytes
	 * @param stream		Stream to write to
	 * @param headerSize 	Size of the protocol buffer header
	 * @throws IOException
	 */
	protected void WriteInitialHeader(OutputStream stream,int headerSize) throws IOException {
		DataOutputStream oOutput = new DataOutputStream(stream);
		oOutput.writeByte(PB_REQUEST_FIRST_BYTE);
		oOutput.writeInt(headerSize);
		oOutput.flush();
	}
	
	/**
	 * Sets the recv buffer to the default size (if neccesary)
	 */
	private void setupRecvBuffer()
	{
		try 
		{
			setupRecvBuffer( DEFAULT_RECV_BUFFER_SIZE );
		}
		catch( Exception e )
		{
			Log.w(
					"Syncro", 
					"Default Syncro buffer size is greater than maximum" 
					);
		}
	}
	
	/**
	 * Sets up the recv buffer to the specified size (if neccesarry)
	 * @param size	The minimum size for the recv buffer
	 * @throws Exception 
	 */
	private void setupRecvBuffer(int size) throws Exception
	{
		if( m_recvBuffer == null || m_recvBuffer.length < size )
		{
			if( size > MAX_RECV_BUFFER_SIZE )
			{
				throw new Exception( 
						"Attempted to create buffer > max buffer size" 
						);
			}
			m_recvBuffer = new byte[size];
		}
	}
}
