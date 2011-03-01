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

#include "PBRequestHandler.h"
#include "PBResponseSendHandler.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <libsyncro/comms.h>
#include <iostream>

namespace syncro
{

using std::string;
using kode::utils::FromJavaEndian;

CPBRequestHandler::CPBRequestHandler( 
	CTCPConnection::TPointer inpConn, 
	CBasePBResponseFactory::TPointer inpResponseFactory, 
	ResponseCallback responseCallback
	) : 
m_pConn( inpConn ), 
m_pResponseFactory( inpResponseFactory ),
m_getResponse( responseCallback )
{
	m_fCloseConnection = false;
	m_pSendHandler = CPBResponseSendHandler::Create( m_pConn );
	ResetVariables();		//don't know if this is needed, but can't hurt much
}

void CPBRequestHandler::ResetVariables()
{
	m_oHeader.Clear();
	m_nBufferReadSoFar = 0;
}

bool CPBRequestHandler::CanHandleReceive( const TCharBuffer& inoBuffer )
{
	unsigned int nHeadSize = comms::PacketHeader::BYTE_SIZE;
	//TODO: Make this stuff use libsyncro's PacketHeader stuff at some point

	//TODO: add stuff in here in case we've read more than we need to
	bool fContinue;
	do
	{
		fContinue = false;
		if(( m_nBufferReadSoFar > 0 ) && m_oHeader.IsInitialized() )
		{
			//We have read in a header already, so we're just waiting for the rest of the data
			int nTotalSubpacketSize = 0;
			for( int nSubPacket = 0; nSubPacket < m_oHeader.subpacket_sizes_size(); nSubPacket++ )
			{
				nTotalSubpacketSize += m_oHeader.subpacket_sizes( nSubPacket );
			}
			if( inoBuffer.nSize >= ( m_nBufferReadSoFar + nTotalSubpacketSize ) )
				return true;
		}
		else if(( m_nBufferReadSoFar == 0 ) && ( inoBuffer.nSize > nHeadSize ) )
		{
			if( inoBuffer.aBuffer[0] == comms::PB_REQUEST_FIRST_BYTE )
			{
				unsigned int nExpectedHeaderSize = ( *( int* )( &inoBuffer.aBuffer[1] ) );
				nExpectedHeaderSize = FromJavaEndian( nExpectedHeaderSize );
				if( inoBuffer.nSize >= nExpectedHeaderSize + nHeadSize )
				{
					m_oHeader.ParseFromArray( &inoBuffer.aBuffer[ nHeadSize ], nExpectedHeaderSize );
					if( !m_oHeader.IsInitialized() )
						throw std::runtime_error( "Received invalid/incomplete PB Request Header" );
					m_nBufferReadSoFar = nHeadSize + nExpectedHeaderSize;
					fContinue = true;  //Run another pass, in case we have all the data we need already
				}
			}
		}
	}
	while( fContinue );

	return false;
}

bool CPBRequestHandler::HandleReceive( const TCharBuffer& inoBuffer )
{
	using std::vector;
	using google::protobuf::io::ArrayInputStream;
	using boost::shared_ptr;

	int numSubpackets = m_oHeader.subpacket_sizes_size();

	InputStreamList subpackets( numSubpackets );
	vector< shared_ptr<ArrayInputStream> > aMemoryManSubpackets( numSubpackets );
	int nCurrentPosition = m_nBufferReadSoFar;
	shared_ptr<ArrayInputStream> pAIS;
	for( int subpacketNum = 0; subpacketNum < m_oHeader.subpacket_sizes_size(); subpacketNum++ )
	{
		int nSize = m_oHeader.subpacket_sizes( subpacketNum );
		pAIS.reset( new ArrayInputStream( &inoBuffer.aBuffer[nCurrentPosition], nSize ) );
		nCurrentPosition += nSize;
		aMemoryManSubpackets[ subpacketNum ] = pAIS;
		subpackets[ subpacketNum ] = pAIS.get();
	}

	try
	{
#if 0
		CBasePBResponse::TPointer pResponse = m_pResponseFactory->CreateResponse( m_oHeader.packet_type(), aSubpackets );
#endif
		CBasePBResponse::TPointer response = 
			m_getResponse( m_oHeader.packet_type(), &subpackets );

		dynamic_cast< CPBResponseSendHandler& >( 
			*m_pSendHandler 
			).SetPBResponse( response );

		m_pConn->Send( m_pSendHandler );
	}
	catch( const authentication_exception& )
	{
		//TODO: do we want to return false here?
		m_fCloseConnection = true;
		m_fatalError = true;
		std::cout << "Authentication failed for " + m_pConn->ClientIP() + "\n";
	}

	ResetVariables();
	return true;
};

};		//namespace syncro
