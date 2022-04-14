#include "tcp_server.hpp"

#include <functional>
#include <boost/asio.hpp>

#include "log.hpp"
#include "tcp_client.hpp"

using namespace std::placeholders;
using namespace boost;

namespace mmocli
{

tcp_server::tcp_server(unsigned short port, asio::io_context* io_context)
    : io_context_(io_context)
    , acceptor_(*io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 2222))
    , new_client_socket_(*io_context_)
    , accepting_(false)
{
    do_accept();
}

tcp_server::~tcp_server()
{
    //acceptor_.cancel();
    //acceptor_.close();
    //accepting_ = false;
    //for (auto& tcp_client : tcp_clients_)
    //    tcp_client->close();
}

void tcp_server::do_accept()
{
    acceptor_.async_accept(new_client_socket_, std::bind(&tcp_server::on_accept, this, _1));
    accepting_ = true;
}

void tcp_server::on_accept(boost::system::error_code const& error)
{
    if (error)
    {
        *log_stream << log("tcp server", "accept", color::red) << error.value() <<  ":" << error.message() << endl;
        accepting_ = false;
        return;
    };

    clients_.emplace_back(io_context_, std::move(new_client_socket_), this);

    *log_stream << log("tcp server", "accept", color::blue) << "new client accepted" << endl;

    do_accept();
}

bool tcp_server::accepting() const
{
    return accepting_;
}

std::list<tcp_client> const& tcp_server::tcp_clients() const
{
    return clients_;
}

void tcp_server::remove_client(tcp_client* client)
{
    std::lock_guard lock(clients_mutex_);
    clients_.remove_if([client ](tcp_client const& c) -> bool
        {
            return &c == client;
        });
}

} // namespace mmocli
