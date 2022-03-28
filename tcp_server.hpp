#pragma once

#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

namespace mmocli
{

class tcp_server: public std::enable_shared_from_this<tcp_server>
{
private:
	boost::asio::io_context& io_context_;
	boost::asio::ip::tcp::endpoint local_endpoint_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::tcp::socket new_client_socket_;

public:
	tcp_server(boost::asio::io_context& io_context, unsigned short port);

public:
	void start();
	void shutdown();

	void do_accept();
	void on_accept(boost::system::error_code const& error);
};

}
