#ifndef _SERVER_COMMS_H_
#define _SERVER_COMMS_H_

#include <boost\asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>

class CTCPConnection : public boost::enable_shared_from_this<CTCPConnection> {
public:
	typedef boost::shared_ptr<CTCPConnection> TPointer;

	static TPointer CreateConnection(boost::asio::io_service& inoIOService) {
		return TPointer( new CTCPConnection( inoIOService ) );
	};

	void StartRecv(int inBytes);
	bool IsRecvFinished(const boost::system::error_code& inoError, std::size_t innBytesSoFar);
	void FinishedRecv(const boost::system::error_code& inoError, std::size_t innBytes );

	void SendMsg();

	boost::asio::ip::tcp::socket& GetSocket() { return m_oSocket; };

private:
	CTCPConnection(boost::asio::io_service& inoIOService);

	boost::asio::ip::tcp::socket m_oSocket;
	std::vector<unsigned char> m_aBuffer;
	std::size_t m_nWaitingRecv;
};

class CServerComms {
public:
	CServerComms(boost::asio::io_service& inoIOService);
	~CServerComms();
private:
	CServerComms(const CServerComms& inoServerComms);
	boost::asio::ip::tcp::acceptor m_oAcceptor;

	void StartAccept();
	void HandleAccept(CTCPConnection::TPointer inpConnection, const boost::system::error_code& inoError);
};

#endif