#pragma once

#include "config.hpp"

#include <vector>
#include <memory>
#include <mutex>

#include <boost/asio/ip/tcp.hpp>

namespace mmocli
{

class tcp_client;

class MMOCLI_API tcp_server : public std::enable_shared_from_this<tcp_server>
{
    friend class proxy;
    friend class tcp_client;
public:
    using clients_container             = std::vector<std::shared_ptr<tcp_client>>;
    using pointer                       = std::shared_ptr<tcp_server>;
private:
    boost::asio::io_context&              io_context_;
    boost::asio::ip::tcp::socket          new_client_socket_;
    clients_container                     clients_;
    mutable std::mutex                    clients_mutex_;
    bool                                  accepting_;
    boost::asio::ip::tcp::acceptor        acceptor_;

public:
                                          tcp_server(unsigned short port, boost::asio::io_context& io_context);
private:
    void                                  do_accept();
    void                                  on_accept(boost::system::error_code const& error);
    void                                  remove_client(clients_container::value_type client);
public:
    void                                  start();
    bool                                  accepting() const;
};

} // namespace mmocli
