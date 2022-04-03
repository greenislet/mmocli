#pragma once

#include "config.hpp"

#include <list>
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include "tcp_client.hpp"

namespace mmocli
{

class MMOCLI_API tcp_server: public std::enable_shared_from_this<tcp_server>
{
private:
    bool                                     accepting_;
    std::shared_ptr<boost::asio::io_context> io_context_p_;
    boost::asio::ip::tcp::acceptor           acceptor_;
    boost::asio::ip::tcp::socket             new_client_socket_;
    std::list<std::shared_ptr<tcp_client>>   tcp_clients_;
public:
                                             tcp_server(std::shared_ptr<boost::asio::io_context> io_context, unsigned short port);
public:
    void                                     start();
    void                                     shutdown();
private:
    void                                     do_accept();
    void                                     on_accept(boost::system::error_code const& error);
public:
    bool                                     accepting() const;
};

} // namespace mmocli
