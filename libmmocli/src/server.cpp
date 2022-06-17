#include "server.hpp"

#include <list>

#include "log.hpp"

using namespace std::literals::chrono_literals;
using namespace boost;

namespace mmocli
{

server::server(asio::io_context& io_context, unsigned short port, std::chrono::steady_clock::duration const& tick_duration)
    : io_context_(&io_context)
    , tcp_server_(std::make_shared<tcp_server>(port, io_context))
    , game_server_(std::make_shared<game_server>(io_context))
{
    ioc_allocated_ = false;
}

void server::start()
{
    tcp_server_->start();
    game_server_->start();
}

server::server(unsigned short port, std::chrono::steady_clock::duration const& tick_duration)
    : server(*new asio::io_context, port, tick_duration)
{
    ioc_allocated_ = true;
}

server::~server()
{
    if (!ioc_allocated_)
        io_context_.release();
    else
        io_context_->stop();
}

server::futures_container server::launch(unsigned int nb_threads)
{
    futures_.clear();
    futures_.reserve(nb_threads);

    for (unsigned int i = 0; i < nb_threads; ++i)
    {
        asio::io_context* ptr = io_context_.get();
        future fut = std::async(std::launch::async, [ptr]
            {
                ptr->run();
            });
        std::shared_ptr<std::future<void>> fut_p = std::make_shared<future>(std::move(fut));
        futures_.emplace_back(fut_p);
    }

    return futures_;
}

void server::wait()
{
    for (future_pointer const& fut_p : futures_)
        fut_p->get();
}

} // namespace mmocli
