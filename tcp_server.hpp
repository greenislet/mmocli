#pragma once

#include "config.hpp"

#include <list>
#include <memory>
#include <mutex>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

//#include "tcp_client.hpp"

namespace mmocli
{

class tcp_client;

class MMOCLI_API tcp_server
{
private:
    boost::asio::io_context*       io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket   new_client_socket_;
    std::list<tcp_client>          clients_;
    mutable std::mutex             clients_mutex_;
    bool                           accepting_;
public:
                                   tcp_server(unsigned short port, boost::asio::io_context* io_context);
                                   ~tcp_server();
private:
    void                           do_accept();
    void                           on_accept(boost::system::error_code const& error);
public:
    bool                           accepting() const;
    std::list<tcp_client> const&   tcp_clients() const;
    void                           remove_client(tcp_client* client);
};

} // namespace mmocli
