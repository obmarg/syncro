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
