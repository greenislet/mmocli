#pragma once

#include "config.hpp"

#include <memory>
#include <thread>
#include <vector>

#include <boost/asio/io_context.hpp>

#include "tcp_server.hpp"

namespace mmocli
{

MMOCLI_API void loop(std::shared_ptr<std::vector<std::future<void>>> futures_p);

struct MMOCLI_API resources
{
    std::shared_ptr<std::vector<std::future<void>>> futures_p;
    std::shared_ptr<boost::asio::io_context> io_context_p;
    std::shared_ptr<tcp_server> tcp_server_p;
};

MMOCLI_API resources launch();

} // namespace mmocli
