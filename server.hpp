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
    using                          future_t            = std::future<void>;
    using                          futures_container_t = std::vector<future_t>;
private:
    std::unique_ptr<boost::asio::io_context>              io_context_;
    bool                           ioc_allocated_;
    tcp_server                     tcp_server_;
    game_server                    game_server_;
public:
                                   server(boost::asio::io_context* io_context, unsigned short port = 2222, std::chrono::steady_clock::duration const& tick_duration = 250ms);
                                   server(unsigned short port = 2222, std::chrono::steady_clock::duration const& tick_duration = 250ms);
                                   ~server();
public:
    futures_container_t            launch(unsigned int nb_threads = std::thread::hardware_concurrency());
};

} // mmocli
