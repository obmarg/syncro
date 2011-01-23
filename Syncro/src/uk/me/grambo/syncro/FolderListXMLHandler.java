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

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import android.database.sqlite.SQLiteStatement;

public class FolderListXMLHandler extends DefaultHandler {
	private static final String FOLDERS_TAG = "Folders";
	private static final String FOLDER_TAG = "Folder";
	
	private boolean m_fInFoldersTag;
	private boolean m_fInFolderTag;
	private StringBuilder m_oBuilder;
	private int m_nCurrentFolderID;
	
	private SQLiteStatement m_oInsertStatement;
	
	public FolderListXMLHandler(SQLiteStatement inoInsertStatement) {
		m_oInsertStatement = inoInsertStatement;
	}
	
	@Override
	public void characters(char[] ch, int start, int length)
	throws SAXException {
		super.characters(ch, start, length);
		if( m_fInFolderTag ) {
			m_oBuilder.append(ch,start,length);
		}
	}

	@Override
	public void endElement(String uri, String localName, String name)
	throws SAXException {
		super.endElement(uri, localName, name);
		if( m_fInFoldersTag ) {
			if( localName.equalsIgnoreCase(FOLDER_TAG) ) {
				m_fInFolderTag = false;
				m_oInsertStatement.bindLong(1,m_nCurrentFolderID);
				m_oInsertStatement.bindString(2,m_oBuilder.toString());
				m_oInsertStatement.bindString(3,m_oBuilder.toString());
				m_oInsertStatement.executeInsert();
				m_oBuilder.setLength(0);
			}
		}
		if (localName.equalsIgnoreCase(FOLDERS_TAG)){
			m_fInFoldersTag = false;
		}
		
	}

	@Override
	public void startDocument() throws SAXException {
		super.startDocument();
		m_fInFoldersTag = false;
		m_fInFolderTag = false;
		m_oBuilder = new StringBuilder();
	}

	@Override
	public void startElement(String uri, String localName, String name,
			Attributes attributes) throws SAXException {
		super.startElement(uri, localName, name, attributes);
		if( m_fInFoldersTag ) {
			if( localName.equalsIgnoreCase(FOLDER_TAG) ) {
				m_nCurrentFolderID = Integer.parseInt(attributes.getValue(0));
				m_fInFolderTag = true;
			}
		}
		if (localName.equalsIgnoreCase(FOLDERS_TAG)){
			m_fInFoldersTag = true;
		}
	}
}
