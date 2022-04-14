#pragma once

#include <array>
#include <memory>

#include <boost/asio.hpp>

#include "tcp_server.hpp"

namespace mmocli
{

class MMOCLI_API tcp_client
{
private:
    boost::asio::io_context*     io_context_;
    boost::asio::ip::tcp::socket socket_;
    std::array<char, 1>          receive_buffer_;
    tcp_server*                  tcp_server_;

public:
                                 tcp_client(boost::asio::io_context* io_context, boost::asio::ip::tcp::socket&& socket, tcp_server* tcp_server);
private:
    void                         do_receive();
    void                         on_receive(boost::system::error_code const& error);
};

} // namespace mmocli