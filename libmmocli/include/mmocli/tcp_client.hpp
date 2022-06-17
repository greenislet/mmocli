#pragma once

#include "config.hpp"

#include <array>
#include <memory>
#include <queue>

#include <boost/asio.hpp>

namespace mmocli
{

class tcp_server;

class MMOCLI_API tcp_client : public std::enable_shared_from_this<tcp_client>
{
    friend class proxy;
public:
    using pointer              = std::shared_ptr<tcp_client>;
    using read_buffer          = std::array<char, 100>;
    using message              = std::string;
    using message_container    = std::queue<message>;
    using string_builder       = std::ostringstream;
private:
    std::string                  addr_str_;
    boost::asio::io_context&     io_context_;
    boost::asio::ip::tcp::socket socket_;
    read_buffer                  read_buffer_;
    string_builder               message_builder_;
    message_container            messages_;
    mutable std::mutex           messages_mutex_;
    tcp_server&                  tcp_server_;

public:
                                 tcp_client(boost::asio::io_context& io_context, boost::asio::ip::tcp::socket&& socket, tcp_server& tcp_server);
public:
    void                         start();
private:
    void                         do_receive();
    void                         on_receive(boost::system::error_code const& error, size_t bytes_transferred);
};

} // namespace mmocli
