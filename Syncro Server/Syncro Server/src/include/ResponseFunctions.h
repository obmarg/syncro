/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	This file contains definitions required for creating protocol buffer
	responses

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

#ifndef SYNCRO_RESPONSE_FUNCTIONS_H_
#define SYNCRO_RESPONSE_FUNCTIONS_H_

#include "UserSession.h"
#include "BasePBResponse.h"
#include "common.h"
#include <kode/variantresponsefactory.h>
#include <boost/function.hpp>

namespace syncro {
namespace server {

//
//	\brief	A response function taking just some packets.
//
typedef boost::function< 
	CBasePBResponse::TPointer ( InputStreamListPtr ) 
> BasicResponseFunction;

//
//	\brief	A response function taking multiple possible packet types
//
typedef boost::function< 
	CBasePBResponse::TPointer ( InputStreamListPtr, int ) 
> MultiResponseFunction;

//
//	\brief	A response function taking some user data
//
typedef boost::function<
	CBasePBResponse::TPointer ( InputStreamListPtr, UserSession& )
> SessionResponseFunction;

//
//	\brief	A response function taking some user data with multiple possible
//			packet types
//
typedef boost::function<
	CBasePBResponse::TPointer ( InputStreamListPtr, UserSession&, int )
> SessionMultiResponseFunction;


//
//	\brief	A variant of all the types of functions PBResponseFactory
//			deals with
//
typedef boost::variant<
	BasicResponseFunction,
	MultiResponseFunction,
	SessionResponseFunction,
	SessionMultiResponseFunction
> ResponseFunctionTypes;

class PBResponseFactory;

//
//	\brief	The base class for our PBResponseFactory
//
typedef kode::VariantResponseFactory< 
	ResponseFunctionTypes, 
	syncro::server::PBResponseFactory,
	InputStreamListPtr,
	CBasePBResponse::TPointer 
> PBResponseFactoryBase;

//
//	\brief	Registers a BasicResponseFunction with a ResponseFactory
//
typedef kode::ResponseRegister< 
	PBResponseFactoryBase, 
	BasicResponseFunction 
> RegisterBasicResponse;

//
//	\brief Registers a MultiResponseFunction with a ResponseFactory
//
typedef kode::ResponseRegister< 
	PBResponseFactoryBase, 
	MultiResponseFunction 
> RegisterMultiResponse;

//
//	\brief Registers a SessionResponseFunction with a ResponseFactory
//
typedef kode::ResponseRegister< 
	PBResponseFactoryBase, 
	SessionResponseFunction 
> RegisterSessionResponse;

//
//	\brief Registers a SessionMultiResponseFunction with a ResponseFactory
//
typedef kode::ResponseRegister< 
	PBResponseFactoryBase, 
	SessionMultiResponseFunction 
> RegisterSessionMultiResponse;


}	// namespace server
}	// namespace syncro


#endif