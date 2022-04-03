#include "tcp_client.hpp"

#include "log.hpp"

using namespace std::placeholders;

namespace mmocli
{

tcp_client::tcp_client(std::shared_ptr<boost::asio::io_context> io_context_p, asio::ip::tcp::socket&& socket)
    : io_context_p_(io_context_p)
    , socket_(std::move(socket))
{}

void tcp_client::start()
{
    do_receive();
}

void tcp_client::close()
{
    socket_.close();
}

void tcp_client::do_receive()
{
    socket_.async_receive(asio::buffer(receive_buffer_), std::bind(&tcp_client::on_receive, shared_from_this(), _1));
}

void tcp_client::on_receive(boost::system::error_code const& error)
{
    if (error)
    {
        if (error.value() != 995 && error.value() != 1236 && error.value() != 10009)
            MMOCLI_LOG_RED("client", "on receive", error.value() << ":" << error.message());

        socket_.close();

        return;
    };

    MMOCLI_LOG_COLOR(MMOCLI_CYAN, "client", "on receive", receive_buffer_[0]);

    do_receive();
}

} // namespace mmocli
