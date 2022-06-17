#include "tests.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <sstream>

//#include "on_exit.hpp"
//#include "server.hpp"
//#include "game_server.hpp"
//#include "tcp_server.hpp"
//#include "tcp_client.hpp"
//#include "log.hpp"

using namespace boost;

using namespace mmocli;

namespace mmocli_tests
{

TEST_F(test_tcp_server, connect_disconnect)
{
    init();
    server->start();

    add_sockets(100);
    connect_sockets();

    ASSERT_NO_THROW(ASSERT_EQ(200, io_context.run_for(env::wait_dur_long)));

    ASSERT_EQ(100, proxy::get_clients(server).size());
    ASSERT_TRUE(server->accepting());

    close_sockets();

    ASSERT_NO_THROW(ASSERT_EQ(100, io_context.run_for(env::wait_dur_long)));

    ASSERT_EQ(0, proxy::get_clients(server).size());
    ASSERT_TRUE(server->accepting());

    // ---------------------

    clear_sockets();
    add_sockets(100);
    connect_sockets();

    std::vector<std::future<size_t>> futs;
    size_t jobs = 10;
    for (size_t i = 0; i < jobs; ++i)
        futs.emplace_back(std::async(std::launch::async, [this]() -> size_t
            {
                size_t n;
                EXPECT_NO_THROW(n = io_context.run_for(env::wait_dur_long));
                return n;
            }));

    std::this_thread::sleep_for(env::wait_dur_long);

    ASSERT_EQ(100, proxy::get_clients(server).size());
    ASSERT_TRUE(server->accepting());

    size_t total = 0;
    for (auto& fut : futs)
        ASSERT_NO_THROW(total += fut.get());

    // 100 async_connect handlers + 100 server on_accept handlers
    ASSERT_EQ(100 + 100, total);

    close_sockets();

    ASSERT_NO_THROW(ASSERT_EQ(100, io_context.run_for(env::wait_dur_long)));

    ASSERT_EQ(0, proxy::get_clients(server).size());
    ASSERT_TRUE(server->accepting());

}

TEST_F(test_tcp_server, write)
{
    init();
    server->start();
    add_sockets(100);
    connect_sockets();

    ASSERT_NO_THROW(ASSERT_EQ(200, io_context.run_for(env::wait_dur_long)));

    std::map<unsigned short, std::string> messages;

    for (auto& socket : sockets)
    {
        std::string buffer;
        buffer.reserve(100);
        for (size_t i = 0; i < 100; ++i)
            buffer += 'a' + ::rand() % 26;
        socket.send(asio::buffer(buffer));
        messages[socket.local_endpoint().port()] = std::move(buffer);
    }

    std::vector<std::future<size_t>> futs;
    size_t jobs = 10;
    for (size_t i = 0; i < jobs; ++i)
        futs.emplace_back(std::async(std::launch::async, [this]() -> size_t
            {
                size_t n = 0;
                EXPECT_NO_THROW(n = io_context.run_for(env::wait_dur_long));
                return n;
            }));

    std::this_thread::sleep_for(env::wait_dur_long);

    ASSERT_EQ(100, proxy::get_clients(server).size());
    ASSERT_TRUE(server->accepting());

    size_t total = 0;
    for (auto& fut : futs)
        ASSERT_NO_THROW(total += fut.get());

    ASSERT_EQ(100, total);

    for (auto& client : proxy::get_clients(server))
    {
        ASSERT_TRUE(proxy::get_messages(client).empty());
        unsigned short port = proxy::get_socket(client).remote_endpoint().port();
        ASSERT_TRUE(messages.contains(port));
        ASSERT_EQ(messages[port], proxy::get_message_builder(client).str());
    }

    for (auto& socket : sockets)
        socket.send(asio::buffer({ '\0' }));

    futs.clear();
    for (size_t i = 0; i < jobs; ++i)
        futs.emplace_back(std::async(std::launch::async, [this]() -> size_t
            {
                size_t n = 0;
                EXPECT_NO_THROW(n = io_context.run_for(env::wait_dur_long));
                return n;
            }));

    std::this_thread::sleep_for(env::wait_dur_long);

    ASSERT_EQ(100, proxy::get_clients(server).size());
    ASSERT_TRUE(server->accepting());

    total = 0;
    for (auto& fut : futs)
        ASSERT_NO_THROW(total += fut.get());

    ASSERT_EQ(100, total);

    for (auto& client : proxy::get_clients(server))
    {
        ASSERT_TRUE(proxy::get_message_builder(client).str().empty());
        ASSERT_EQ(1, proxy::get_messages(client).size());
        unsigned short port = proxy::get_socket(client).remote_endpoint().port();
        ASSERT_TRUE(messages.contains(port));
        ASSERT_EQ(messages[port], proxy::get_messages(client).back());
    }
}

}

int main(int argc, char** argv, char** env)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new mmocli_tests::env(env));
    return RUN_ALL_TESTS();
}
