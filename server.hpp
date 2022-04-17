#pragma once

#include "config.hpp"

#include <memory>
#include <vector>
#include <future>
#include <chrono>

#include <boost/asio.hpp>

#include "tcp_server.hpp"
#include "game_server.hpp"

using namespace std::literals::chrono_literals;

namespace mmocli
{

class MMOCLI_API server
{
public:
    using future                           = std::future<void>;
    using future_pointer                   = std::shared_ptr<future>;
    using futures_container                = std::vector<future_pointer>;
private:
    std::unique_ptr<boost::asio::io_context> io_context_;
    tcp_server::pointer                      tcp_server_;
    game_server::pointer                     game_server_;
    bool                                     ioc_allocated_;
    futures_container                        futures_;
public:
                                             server(boost::asio::io_context& io_context, unsigned short port = 2222, std::chrono::steady_clock::duration const& tick_duration = 250ms);
                                             server(unsigned short port = 2222, std::chrono::steady_clock::duration const& tick_duration = 250ms);
                                             ~server();
public:
    void                                     start();
    futures_container                        launch(unsigned int nb_threads = std::thread::hardware_concurrency());
    void                                     wait();
};

} // mmocli
