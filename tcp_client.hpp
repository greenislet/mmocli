#pragma once

#include <array>
#include <memory>

#include <boost/asio.hpp>

namespace asio = boost::asio;

namespace mmocli
{

class tcp_client : public std::enable_shared_from_this<tcp_client>
{
private:
    std::shared_ptr<boost::asio::io_context> io_context_p_;
    asio::ip::tcp::socket                    socket_;
    std::array<char, 1>                      receive_buffer_;

public:
                                             tcp_client(std::shared_ptr<boost::asio::io_context> io_context_p, asio::ip::tcp::socket&& socket);
public:
    void                                     start();
    void                                     close();
private:
    void                                     do_receive();
    void                                     on_receive(boost::system::error_code const& error);
};

} // namespace mmocli