#include "tcp_server.hpp"

#include <functional>
#include <boost/asio.hpp>

#include "log.hpp"
#include "tcp_client.hpp"

using namespace std::placeholders;
using namespace boost;

namespace mmocli
{

tcp_server::tcp_server(unsigned short port, asio::io_context& io_context)
    : io_context_(io_context)
    , new_client_socket_(io_context_)
    , accepting_(false)
    , acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 2222))
{
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
}

void tcp_server::do_accept()
{
    acceptor_.async_accept(new_client_socket_, std::bind(&tcp_server::on_accept, shared_from_this(), _1));
    accepting_ = true;
}

void tcp_server::on_accept(boost::system::error_code const& error)
{
    if (error)
    {
        log("tcp server", "accept", color::red) << error.value() << ":" << error.message() << endl;
        accepting_ = false;
        return;
    };

    clients_.emplace_back(std::make_shared<tcp_client>(io_context_, std::move(new_client_socket_), *this));

    clients_.back()->start();

    log("tcp server", "accept", color::blue) << "new client accepted" << endl;

    do_accept();
}

void tcp_server::start()
{
    do_accept();
}

bool tcp_server::accepting() const
{
    return accepting_;
}

void tcp_server::remove_client(clients_container::value_type client)
{
    std::lock_guard lock(clients_mutex_);
    clients_container::const_iterator it = std::remove(clients_.begin(), clients_.end(), client);
    clients_.erase(it, clients_.cend());
}

} // namespace mmocli
