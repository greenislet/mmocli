#include "tcp_server.hpp"

#include <functional>

#include <boost/asio.hpp>

#include "log.hpp"

using namespace std::placeholders;

namespace mmocli
{

tcp_server::tcp_server(std::shared_ptr<boost::asio::io_context> io_context_p, unsigned short port)
    : io_context_p_(io_context_p)
    , acceptor_(*io_context_p, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 2222))
    , new_client_socket_(*io_context_p)
    , accepting_(false)
{}

void tcp_server::start()
{
    do_accept();
}

void tcp_server::shutdown()
{
    acceptor_.cancel();
    acceptor_.close();
    accepting_ = false;
    for (auto& tcp_client : tcp_clients_)
        tcp_client->close();
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
        if (error.value() != 995 && error.value() != 1236 && error.value() != 10009)
            MMOCLI_LOG_COLOR(MMOCLI_RED, "server", "accept", error.value() << ":" << error.message());

        new_client_socket_.close();

        return;
    };

    auto conn = std::make_shared<tcp_client>(io_context_p_, std::move(new_client_socket_));

    tcp_clients_.emplace_back(conn);

    tcp_clients_.back()->start();

    MMOCLI_LOG_COLOR(MMOCLI_BLUE, "server", "accept", "new client accepted");

    do_accept();
}

bool tcp_server::accepting() const
{
    return accepting_;
}

} // namespace mmocli
