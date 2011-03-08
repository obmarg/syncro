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

#ifndef _PROTOCOL_BUFFER_REQUEST_HANDLER_H_
#define _PROTOCOL_BUFFER_REQUEST_HANDLER_H_

#include "common.h"
#include "ServerComms.h"
#include "BasePBResponse.h"
#include <libsyncro/protocol_buffers/header.pb.h>
#include <boost/function.hpp>
#include <iterator>

namespace syncro
{

class CPBRequestHandler : public CReceiveHandler
{
public:
	typedef boost::function< 
		BasePBResponse::TPointer (	const unsigned int, InputStreamListPtr )
	> ResponseCallback;

	virtual ~CPBRequestHandler() {};

	virtual bool CanHandleReceive( const TCharBuffer& inoBuffer );
	virtual bool HandleReceive( const TCharBuffer& inoBuffer );
	virtual bool CanRemove()
	{
		return m_fCloseConnection;
	};

	static CReceiveHandler::TPointer Create( 
		CTCPConnection::TPointer inpConn, 
		ResponseCallback responseCallback 
		)
	{
		return CReceiveHandler::TPointer( 
			static_cast<CReceiveHandler*>( 
				new CPBRequestHandler( 
					inpConn, 
					responseCallback 
					) 
				) 
			);
	}
protected:
	bool m_fCloseConnection;

	CTCPConnection::TPointer m_pConn;
	ResponseCallback m_getResponse;

	CSendHandler::TPointer m_pSendHandler;

	pb::PacketHeader m_oHeader;
	unsigned int m_nBufferReadSoFar;

	CPBRequestHandler( 
		CTCPConnection::TPointer inpConn, 
		ResponseCallback responseCallback
		);

	void ResetVariables();
};

};		//namespace syncro

#endif
