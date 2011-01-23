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

#ifndef _XML_BUILDER_H_
#define _XML_BUILDER_H_

#include "common.h"
#include "tinyxml.h"
#include <boost/shared_ptr.hpp>

class TiXmlDocument;

namespace syncro
{

class CXMLBuilder
{
public:
	CXMLBuilder();
	~CXMLBuilder();

	void SaveDocAsFile( std::string inoFile );

	bool GetFolderXML();
	bool GetFolderContentsXML( int innFolderID );
private:
	TiXmlDocument m_oXML;

	void ProcessFolder( TiXmlElement& inoParentElement, boost::shared_ptr<class CFolder> inoFolder, bool infIsRoot = false );

};

class CXMLOutput
{
public:
	CXMLOutput( CXMLBuilder& inoBuilder ) : m_oBuilder( inoBuilder )
	{

	}
	CXMLOutput( const CXMLOutput& inoOther ) : m_oBuilder( inoOther.m_oBuilder ) {};

	void operator()( TCharBuffer::TBuff& inoBuffer );
private:
	CXMLBuilder& m_oBuilder;
};

};		//namespace syncro


#endif