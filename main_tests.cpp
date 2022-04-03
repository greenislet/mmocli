#include <chrono>
#include <future>

#include <iostream>
#include <thread>
#include <tuple>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/use_future.hpp>

#include <gtest/gtest.h>

#include "launch.hpp"
#include "log.hpp"
#include "on_exit.hpp"

using namespace std::literals::chrono_literals;

namespace asio = boost::asio;

#define ASSERT_FUTURES()\
do {\
for (auto& fut : *resources.futures_p)\
{\
    ASSERT_TRUE(fut.valid());\
    ASSERT_NO_THROW(fut.get());\
} } while(0)

#define LAUNCH()\
mmocli::resources resources;\
ASSERT_NO_THROW(resources = mmocli::launch());\
mmocli::on_exit oe([&resources] { resources.io_context_p->stop(); })

TEST(tcp_server, all)
{
    {
        MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.launch_shutdown", "launching");

        LAUNCH();

        ASSERT_TRUE(resources.tcp_server_p->accepting());

        MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.launch_shutdown", "shuting down");

        ASSERT_NO_THROW(resources.tcp_server_p->shutdown());
        ASSERT_FALSE(resources.tcp_server_p->accepting());

        ASSERT_FUTURES();
    }

    {
        MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.accept", "launching");

        LAUNCH();

        MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.accept", "waiting for resolve");
        asio::ip::tcp::resolver resolver(*resources.io_context_p);
        std::future<std::tuple<boost::system::error_code, asio::ip::tcp::resolver::results_type>> fut1 = std::async(std::launch::async, [&resolver]
            {
                boost::system::error_code error;
                asio::ip::tcp::resolver::results_type results = resolver.resolve(asio::ip::tcp::resolver::query("localhost", "2222"), error);
                if (error)
                    MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.accept", "resolve: " << error.message());
                return std::make_tuple(error, results);
            });
        ASSERT_EQ(std::future_status::ready, fut1.wait_for(5s));
        std::tuple<boost::system::error_code, asio::ip::tcp::resolver::results_type> results = fut1.get();
        ASSERT_FALSE(std::get<0>(results));

        asio::ip::tcp::socket socket(*resources.io_context_p);
        MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.accept", "waiting for connect");
        std::future<boost::system::error_code> fut2 = std::async(std::launch::async, [&socket, &results]
            {
                boost::system::error_code error;
                socket.connect(*std::get<1>(results), error);
                if (error)
                    MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.accept", "accept: " << error.message());
                return error;
            });

        ASSERT_EQ(std::future_status::ready, fut2.wait_for(5s));
        ASSERT_FALSE(fut2.get());

        MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.accept", "shuting down");
        resources.tcp_server_p->shutdown();

        ASSERT_FUTURES();
    }

    {
        MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.write", "launching");

        LAUNCH();

        asio::ip::tcp::resolver resolver(*resources.io_context_p);

        asio::ip::tcp::resolver::results_type results = resolver.resolve(asio::ip::tcp::resolver::query("localhost", "2222"));

        asio::ip::tcp::socket socket(*resources.io_context_p);
        socket.connect(*results);

        socket.send(asio::buffer("test"));

        std::this_thread::sleep_for(1s);

        MMOCLI_LOG_COLOR(MMOCLI_YELLOW, "test", "tcp_server.write", "shuting down");
        resources.tcp_server_p->shutdown();

        ASSERT_FUTURES();
    }
}
