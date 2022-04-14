#include "tcp_client.hpp"

#include "log.hpp"

using namespace std::placeholders;
using namespace boost;

namespace mmocli
{

tcp_client::tcp_client(boost::asio::io_context* io_context, asio::ip::tcp::socket&& socket, tcp_server* tcp_server)
    : io_context_(io_context)
    , socket_(std::move(socket))
    , tcp_server_(tcp_server)
{
    do_receive();
}

void tcp_client::do_receive()
{
    socket_.async_receive(asio::buffer(receive_buffer_), std::bind(&tcp_client::on_receive, this, _1));
}

std::vector<boost::system::error_code> critical_errors =
{
    boost::asio::error::eof
};

static bool is_critical(boost::system::error_code const& error)
{
    return std::find(critical_errors.cbegin(), critical_errors.cend(), error) == critical_errors.cend();
}

void tcp_client::on_receive(boost::system::error_code const& error)
{

    if (error)
    {
        *log_stream << log("tcp client", "on receive", is_critical(error) ? color::red : color::cyan) << error.value() << ":" << error.message() << endl;
        tcp_server_->remove_client(this);
        return;
    };

    *log_stream << log("tcp client", "on receive", color::cyan) << receive_buffer_[0] << endl;

    do_receive();
}

} // namespace mmocli
