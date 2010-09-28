package uk.me.grambo.syncro;

import java.util.ArrayList;
import java.util.Stack;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import android.database.sqlite.SQLiteStatement;

public class FolderContentsXMLHandler extends DefaultHandler {
	private static final String ROOT_TAG = "RootFolder";
	private static final String FOLDER_TAG = "Folder";
	private static final String FILE_TAG = "File";
	
	private boolean m_fInFileTag;
	
	private ArrayList<RemoteFileHandler> m_aHandlers;
	private Stack<String> m_aPath;
	
	private StringBuilder m_oFilenameBuilder;
	
	public FolderContentsXMLHandler() {
		m_aHandlers = new ArrayList<RemoteFileHandler>();
	}
	
	public void AddFileHandler(RemoteFileHandler inHandler) {
		m_aHandlers.add( inHandler );
	}
	
	@Override
	public void characters(char[] ch, int start, int length)
	throws SAXException {
		super.characters(ch, start, length);
		if( m_fInFileTag ) {
			m_oFilenameBuilder.append(ch,start,length);
		}
	}

	@Override
	public void endElement(String uri, String localName, String name)
	throws SAXException {
		super.endElement(uri, localName, name);
		if( localName.equals(FOLDER_TAG) ) {
			m_aPath.pop();
		} else if( localName.equals(FILE_TAG) ) {
			m_fInFileTag = false;
			StringBuilder oPathBuilder = new StringBuilder();
			for(int n=0;n< m_aPath.size();n++ ) {
				oPathBuilder.append(m_aPath.elementAt(n));
				oPathBuilder.append("/");
			}
			for(int n=0;n < m_aHandlers.size();n++ ) {
				m_aHandlers.get(n).HandleRemoteFile( m_oFilenameBuilder.toString() );
			}
		}
	}

	@Override
	public void startDocument() throws SAXException {
		super.startDocument();
		m_aPath = new Stack<String>();
		m_fInFileTag = false;
	}

	@Override
	public void startElement(String uri, String localName, String name,
			Attributes attributes) throws SAXException {
		super.startElement(uri, localName, name, attributes);
		if( localName.equals(FOLDER_TAG) ) {
			m_aPath.push(attributes.getValue("Name"));
		} else if( localName.equals(FILE_TAG) ) {
			m_fInFileTag = true;
			m_oFilenameBuilder = new StringBuilder();
		}
	}
}
