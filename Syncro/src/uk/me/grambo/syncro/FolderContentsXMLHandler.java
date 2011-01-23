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
	
	private int m_nCurrentFolderId;
	
	private ArrayList<RemoteFileHandler> m_aHandlers;
	private Stack<String> m_aPath;
	
	private StringBuilder m_oFilenameBuilder;
	private int m_nFileSize;
	
	public FolderContentsXMLHandler() {
		m_aHandlers = new ArrayList<RemoteFileHandler>();
	}
	
	public void AddFileHandler(RemoteFileHandler inHandler) {
		m_aHandlers.add( inHandler );
	}
	
	public void SetFolderId( int innFolderId ) {
		m_nCurrentFolderId = innFolderId;
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
				RemoteFileHandler.RemoteFileData oFile = new RemoteFileHandler.RemoteFileData();
				oFile.Filename = oPathBuilder.toString() + m_oFilenameBuilder.toString();
				oFile.FolderId = m_nCurrentFolderId;
				oFile.Size = m_nFileSize;
				m_aHandlers.get(n).HandleRemoteFile( oFile );
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
			m_aPath.push(attributes.getValue(0));//("Name"));
		} else if( localName.equals(FILE_TAG) ) {
			m_fInFileTag = true;
			m_oFilenameBuilder = new StringBuilder();
			m_nFileSize = Integer.parseInt( attributes.getValue("size") );
		}
	}
}
