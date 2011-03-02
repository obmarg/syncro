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

#ifndef _SYNCRO_PB_RESPONSE_FACTORY_H_
#define _SYNCRO_PB_RESPONSE_FACTORY_H_

#include "common.h"
#include "BasePBResponse.h"
#include "FileSendData.h"
#include "FolderMan.h"
#include "BinaryIncomingData.h"
#include "AdminCommandManager.h"
#include "AuthManager.h"
#include "ResponseFunctions.h"
#include <libsyncro/packet_types.h>
#include <kode/variantresponsefactory.h>
#include <boost/scoped_ptr.hpp>

namespace syncro
{

struct sSubpackets
{
	typedef std::vector<unsigned int> TSizeList;
	unsigned int Count;
	TSizeList Sizes;
	const TCharBuffer::TBuff& Buffer;
};

namespace server
{	

//TODO: Move all this shit about at some point?
//		Also, remove the old syncro pb response factory once i've finished
//		The new one

//
//	\brief	Implements the syncro protocol buffer response factory
//			by inheriting from a VariantResponseFactory
//
class PBResponseFactory : 
	public PBResponseFactoryBase,
	public boost::static_visitor<CBasePBResponse::TPointer>
{
public:
	PBResponseFactory() {};

	virtual CBasePBResponse::TPointer CallHandler(
		const SessionResponseFunction& function,
		InputStreamListPtr input
		)
	{
		function( input, m_session );
	}
	
private:
	UserSession			m_session;
};

}	// namespace server

class CSyncroPBResponseFactory : public CBasePBResponseFactory
{
public:
	CSyncroPBResponseFactory( const CSyncroPBResponseFactory& inoOther )
	{
		//TODO: Implement copying of stuff (or make uncopyable?)
	}
	virtual ~CSyncroPBResponseFactory() {};

	static CBasePBResponseFactory::TPointer Create()
	{
		return CBasePBResponseFactory::TPointer( new CSyncroPBResponseFactory() );
	};

	virtual CBasePBResponse::TPointer CreateResponse( const unsigned int innPacketType, InputStreamList& inaInputStreams );
private:
	CSyncroPBResponseFactory();

	void HashOkCallback( const std::string& filename, int64_t sizeHashed );

	typedef boost::scoped_ptr<CFileSendData> TFileSendDataPtr;
	TFileSendDataPtr m_pCurrentSendData;
	boost::scoped_ptr<CFolderMan> m_pFolderMan;
	typedef boost::scoped_ptr<CBinaryIncomingData> TFileRecvDataPtr;
	TFileRecvDataPtr m_pCurrentRecvData;
	boost::scoped_ptr<CAdminCommandManager> m_pAdminCommandMan;

	bool m_fAuthenticated;
	CAuthToken m_oAuthToken;
	CAuthManager m_authMan;
};

}; //namespace syncro
#endif
