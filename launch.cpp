#include "launch.hpp"

#include <exception>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <tuple>

#include <boost/asio/io_context.hpp>

#include "log.hpp"
#include "tcp_server.hpp"

using namespace std::literals::chrono_literals;

namespace mmocli
{

resources launch()
{
    unsigned short  port = 2222;
    unsigned int    nb_threads = 1;// std::thread::hardware_concurrency();


    auto io_context_p = std::make_shared<boost::asio::io_context>();

    auto futures_p = std::make_shared<std::vector<std::future<void>>>();

    auto tcp_server_p = std::make_shared<tcp_server>(io_context_p, port);

    tcp_server_p->start();

    for (unsigned int i = 0; i < nb_threads; ++i)
    {
        std::future<void> fut = std::async(std::launch::async, [io_context_p]
            {
                io_context_p->run();
            });
        futures_p->emplace_back(std::move(fut));
    }

    return { futures_p, io_context_p, tcp_server_p };
}

} // namespace mmocli
