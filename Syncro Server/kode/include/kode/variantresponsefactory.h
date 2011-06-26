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

#ifndef KODE_VARIANT_RESPONSE_FACTORY
#define KODE_VARIANT_RESPONSE_FACTORY

#include <boost/variant.hpp>
#include <map>

namespace kode {

//TODO: Rename base pb response factory
//TODO: Possibly make a generic factory template, then
//		a pb factory typedef, then a variant factory template on top
//		of that?
//TODO: move stuff out of syncro...

///////////////////////////////////////////////////////////////////////////////
//!
//!	\brief	A ResponseFactory template that expects the input data to
//!			be a Variant
//!
//!	\note	This class has not been used much, so may require testing
//!
//!	\tparam	Type		The base type of the handlers
//!	\tparam	Visitor		The type of the visitor
//!	\tparam	InputData	The input variant data type
//!	\tparam	OutputData	The output data type
//!
///////////////////////////////////////////////////////////////////////////////
template<class Type,class Visitor,class InputData,class OutputData>
class VariantResponseFactory
{
public:
	VariantResponseFactory()
		:m_handlers( ms_handlers )
	{ }
	~VariantResponseFactory() { };

	void AddHandler( unsigned int messageId, Type handler )
	{
		m_handlers.insert(
			std::make_pair(messageId,handler)
			);
	}
	static void AddStaticHandler( unsigned int messageId, Type handler )
	{
		ms_handlers.insert(
			std::make_pair( messageId, handler )
			);
	}

	virtual OutputData CreateResponse( 
										const unsigned int packetType, 
										InputData inputData 
										)
	{
		//TODO: Figure out if this iterator is allowed to be const
		typename HandlerMap::const_iterator handlerIt = 
			m_handlers.find( packetType );

		if( handlerIt == m_handlers.end() )
		{
			throw std::runtime_error( 
				"Invalid input packet in VariantResponseFactory::CreateResponse"
				);
		}
		m_packetType = packetType;
		SetInputData( inputData );
		OutputData rv = 
			boost::apply_visitor( GetVisitor(), handlerIt->second );
		ClearInputData();
		return rv;
	}
	virtual Visitor&	GetVisitor()=0;
	virtual void		SetInputData( InputData inputData )=0;
	virtual	void		ClearInputData() {};
protected:
	unsigned int m_packetType;
private:
	typedef std::map<unsigned int,Type> HandlerMap;
	HandlerMap m_handlers;

	static HandlerMap ms_handlers;
};
template<class Type,class Visitor,class InputData,class OutputData>
	typename VariantResponseFactory< Type,Visitor,InputData,OutputData >::HandlerMap
		VariantResponseFactory< Type,Visitor,InputData,OutputData >::ms_handlers;

}	// namespace kode

#endif
