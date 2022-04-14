#include "server.hpp"

#include "log.hpp"

using namespace std::literals::chrono_literals;
using namespace boost;

namespace mmocli
{

server::server(asio::io_context* io_context, unsigned short port, std::chrono::steady_clock::duration const& tick_duration)
    : io_context_(io_context)
    , tcp_server_(port, io_context_.get())
    , game_server_(io_context_.get())
{
    ioc_allocated_ = false;
}

server::server(unsigned short port, std::chrono::steady_clock::duration const& tick_duration)
    : server(new asio::io_context, port, tick_duration)
{
    ioc_allocated_ = true;
}

server::~server()
{
    if (!ioc_allocated_)
        io_context_.release();
}

server::futures_container_t server::launch(unsigned int nb_threads)
{
    server::futures_container_t futures;
    futures.reserve(nb_threads);

    for (unsigned int i = 0; i < nb_threads; ++i)
    {
        auto ptr = io_context_.get();
        server::future_t fut = std::async(std::launch::async, [ptr]
            {
                ptr->run();
            });
        futures.emplace_back(std::move(fut));
    }

    return futures;
}

} // namespace mmocli
