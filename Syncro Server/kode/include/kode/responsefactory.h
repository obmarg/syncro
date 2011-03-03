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

#ifndef KODE_RESPONSE_FACTORY
#define KODE_RESPONSE_FACTORY

#include <map>

namespace kode {

	//TODO: Rename base pb response factory
	//TODO: Possibly make a generic factory template, then
	//		a pb factory typedef, then a variant factory template on top
	//		of that?
	//TODO: move stuff out of syncro...
template<class Type,class InputData,class OutputData>
class ResponseFactory
{
private:
	typedef std::map<unsigned int,Type> HandlerMap;
public:
	ResponseFactory()
		:m_handlers( GetStaticHandlers() )
	{ }
	~ResponseFactory() { };

	void AddHandler( unsigned int messageId, Type handler )
	{
		m_handlers.insert(
			std::make_pair(messageId,handler)
			);
	}
	static void AddStaticHandler( unsigned int messageId, Type handler )
	{
		GetStaticHandlers().insert(
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
				"Invalid input packet in VariantResponseFactory::CreateResponse:"
				);
		}
		m_packetType = packetType;
		return CallHandler( handlerIt->second, inputData );
	}
	virtual OutputData CallHandler( const Type& handler, InputData data )=0;
protected:
	unsigned int m_packetType;
private:
	HandlerMap m_handlers;
	static HandlerMap& GetStaticHandlers()
	{
		static HandlerMap handlerMap;
		return handlerMap;
	}
};

template<class Factory,class Handler>
class ResponseRegister
{
public:
	ResponseRegister( unsigned int packetType, Handler handler )
	{
		Factory::AddStaticHandler( packetType, handler );
	}
};

}	// namespace kode

#endif
