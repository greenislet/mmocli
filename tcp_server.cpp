#include <functional>

#include "tcp_server.hpp"

using namespace std::placeholders;

namespace mmocli
{

tcp_server::tcp_server(boost::asio::io_context& io_context, unsigned short port)
	: io_context_(io_context)
	, local_endpoint_(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), port))
	, acceptor_(io_context)
	, new_client_socket_(io_context)
{}

void tcp_server::start()
{
	acceptor_.open(local_endpoint_.protocol());
	acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
	acceptor_.bind(local_endpoint_);
	acceptor_.listen();

	do_accept();
}

void tcp_server::shutdown()
{
	acceptor_.cancel();
	acceptor_.close();
}

void tcp_server::do_accept()
{
	acceptor_.async_accept(new_client_socket_, std::bind(&tcp_server::on_accept, shared_from_this(), _1));
}

void tcp_server::on_accept(boost::system::error_code const& error)
{

}

} // namespace mmocli
