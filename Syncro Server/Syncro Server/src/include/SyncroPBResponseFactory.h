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
#include "ResponseFunctions.h"
#include <kode/variantresponsefactory.h>
#include <boost/scoped_ptr.hpp>

namespace syncro {
namespace server {	

//
//	\brief	Implements the syncro protocol buffer response factory
//			by inheriting from a VariantResponseFactory
//
class PBResponseFactory : 
	public PBResponseFactoryBase
{
public:
	PBResponseFactory() {};

	virtual BasePBResponse::TPointer CallHandler(
		const SessionResponseFunction& function,
		InputStreamListPtr input
		)
	{
		return function( input, m_session );
	}
	
private:
	UserSession			m_session;
};

}	// namespace server
}	//namespace syncro

#endif
