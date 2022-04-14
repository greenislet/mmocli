#include <gtest/gtest.h>

#include "on_exit.hpp"
#include "server.hpp"
#include "game_server.hpp"

using namespace boost;

TEST(tcp_server, instanciation_destruction)
{
    mmocli::tcp_server* tcp_server;
    asio::io_context io_context;
    ASSERT_NO_THROW(tcp_server = new mmocli::tcp_server(2222, &io_context));
    ASSERT_NO_THROW(delete tcp_server);
}

TEST(tcp_server, connect_one)
{
    asio::io_context io_context;
    mmocli::on_exit stop_on_exit1([&io_context] {io_context.stop(); });
    auto tcp_server = new mmocli::tcp_server(2222, &io_context);

    ASSERT_TRUE(tcp_server->accepting());

    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::resolver::results_type results = resolver.resolve(asio::ip::tcp::resolver::query("127.0.0.1", "2222"));


    auto fut = std::async(std::launch::async, [&io_context] { io_context.run(); });

    ASSERT_TRUE(tcp_server->tcp_clients().empty());

    asio::ip::tcp::socket socket(io_context);
    mmocli::on_exit stop_on_exit2([&io_context] {io_context.stop(); });

    socket.connect(*results);
    std::this_thread::sleep_for(0.1s);

    ASSERT_EQ(1, tcp_server->tcp_clients().size());
    ASSERT_TRUE(tcp_server->accepting());

    ASSERT_NO_THROW(io_context.stop());
    ASSERT_NO_THROW(delete tcp_server);
    ASSERT_NO_THROW(fut.get());
}

TEST(tcp_server, connect_two)
{
    asio::io_context io_context;
    mmocli::on_exit stop_on_exit1([&io_context] {io_context.stop(); });
    auto tcp_server = new mmocli::tcp_server(2222, &io_context);

    ASSERT_TRUE(tcp_server->accepting());

    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::resolver::results_type results = resolver.resolve(asio::ip::tcp::resolver::query("127.0.0.1", "2222"));


    auto fut = std::async(std::launch::async, [&io_context] { io_context.run(); });

    ASSERT_TRUE(tcp_server->tcp_clients().empty());

    asio::ip::tcp::socket socket1(io_context);
    asio::ip::tcp::socket socket2(io_context);
    mmocli::on_exit stop_on_exit2([&io_context] {io_context.stop(); });

    socket1.connect(*results);
    std::this_thread::sleep_for(0.1s);

    ASSERT_EQ(1, tcp_server->tcp_clients().size());
    ASSERT_TRUE(tcp_server->accepting());

    socket2.connect(*results);
    std::this_thread::sleep_for(0.1s);

    ASSERT_EQ(2, tcp_server->tcp_clients().size());
    ASSERT_TRUE(tcp_server->accepting());

    ASSERT_NO_THROW(io_context.stop());
    ASSERT_NO_THROW(delete tcp_server);
    ASSERT_NO_THROW(fut.get());
}

TEST(tcp_server, connect_twenty)
{
    asio::io_context io_context;
    mmocli::on_exit stop_on_exit1([&io_context] {io_context.stop(); });
    auto tcp_server = new mmocli::tcp_server(2222, &io_context);

    ASSERT_TRUE(tcp_server->accepting());

    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::resolver::results_type results = resolver.resolve(asio::ip::tcp::resolver::query("127.0.0.1", "2222"));


    auto fut = std::async(std::launch::async, [&io_context] { io_context.run(); });

    ASSERT_TRUE(tcp_server->tcp_clients().empty());

    std::vector<asio::ip::tcp::socket> sockets;
    mmocli::on_exit stop_on_exit2([&io_context] {io_context.stop(); });

    for (int i = 0; i < 20; ++i)
    {
        sockets.emplace_back(io_context);
        sockets.back().connect(*results);
        std::this_thread::sleep_for(0.01s);

        ASSERT_EQ(i + 1, tcp_server->tcp_clients().size());
        ASSERT_TRUE(tcp_server->accepting());
    }

    ASSERT_NO_THROW(io_context.stop());
    ASSERT_NO_THROW(delete tcp_server);
    ASSERT_NO_THROW(fut.get());
}

TEST(tcp_server, connect_disconnect)
{
    asio::io_context io_context;
    mmocli::on_exit stop_on_exit1([&io_context] {io_context.stop(); });
    auto tcp_server = new mmocli::tcp_server(2222, &io_context);


    ASSERT_TRUE(tcp_server->accepting());

    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::resolver::results_type results = resolver.resolve(asio::ip::tcp::resolver::query("127.0.0.1", "2222"));


    auto fut = std::async(std::launch::async, [&io_context] { io_context.run(); });

    ASSERT_TRUE(tcp_server->tcp_clients().empty());

    asio::ip::tcp::socket socket(io_context);
    mmocli::on_exit stop_on_exit2([&io_context] {io_context.stop(); });
    socket.connect(*results);
    std::this_thread::sleep_for(0.1s);

    ASSERT_EQ(1, tcp_server->tcp_clients().size());
    ASSERT_TRUE(tcp_server->accepting());

    socket.close();
    std::this_thread::sleep_for(0.1s);

    ASSERT_EQ(0, tcp_server->tcp_clients().size());
    ASSERT_TRUE(tcp_server->accepting());

    ASSERT_NO_THROW(io_context.stop());
    ASSERT_NO_THROW(delete tcp_server);
    ASSERT_NO_THROW(fut.get());
}

TEST(game_server, instanciation_destruction)
{
    mmocli::game_server* game_server;
    asio::io_context io_context;
    ASSERT_NO_THROW(game_server = new mmocli::game_server(&io_context));
    ASSERT_NO_THROW(delete game_server);
}

TEST(server, instanciation_destruction)
{
    mmocli::server* server;
    ASSERT_NO_THROW(server = new mmocli::server());
    ASSERT_NO_THROW(delete server);
    asio::io_context io_context;
    ASSERT_NO_THROW(server = new mmocli::server(&io_context));
    ASSERT_NO_THROW(delete server);
}
