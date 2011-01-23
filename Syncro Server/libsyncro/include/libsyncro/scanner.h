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

#ifndef LIBSYNCRO_SCANNER
#define LIBSYNCRO_SCANNER

#include <libsyncro/connection.h>
#include <kode/utils.h>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <set>

namespace syncro
{
namespace client
{

class Scanner
{
public:
	class ServerDetails
	{
	public:
		virtual const std::string& Name() const = 0;
		operator ConnectionDetails()
		{
			return GetConnDetails();
		}
		bool operator<( const ServerDetails& rhs )
		{
			if( this == &rhs )
				return false;
			return ( Name().compare( rhs.Name() ) < 0 );
		}
	protected:
		virtual ConnectionDetails GetConnDetails() const = 0;
	};
	typedef boost::shared_ptr<ServerDetails> ServerDetailsPtr;
	typedef std::set<ServerDetailsPtr, kode::utils::CPointerLessThan> ServerList;

public:
	Scanner();
	~Scanner();

	void Scan( unsigned int timeout );
	void Reset();

	const ServerList& Servers() const
	{
		return m_servers;
	}
private:
	ServerList m_servers;
	std::string m_response;
	std::vector< char > m_responseBuffer;
	boost::asio::ip::udp::endpoint m_sender;
	bool m_finished;

	bool HandleReceive(
	    const boost::system::error_code& error,
	    std::size_t
	);
	bool Finished()
	{
		m_finished = true;
		return true;
	}
};

}	// namespace client
}	// namespace syncro

#endif
