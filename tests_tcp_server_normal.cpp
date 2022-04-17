#include "tests.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <sstream>

#include "on_exit.hpp"
#include "server.hpp"
#include "game_server.hpp"
#include "tcp_server.hpp"
#include "tcp_client.hpp"
#include "log.hpp"

using namespace boost;

namespace mmocli
{

class normal : public tcp_server_helper
{};

TEST_F(normal, instanciation_destruction)
{
    ASSERT_NO_THROW(server = std::make_shared<tcp_server>(2222, io_context));
    ASSERT_NO_THROW(server.reset());
}

TEST_F(normal, start_stop)
{
    server = std::make_shared<tcp_server>(2222, io_context);
    ASSERT_NO_THROW(server->start());
    ASSERT_TRUE(server->accepting());

    ASSERT_NO_THROW(ASSERT_EQ(0, io_context.run_for(env::wait_dur_short)));
    ASSERT_TRUE(server->accepting());

    ASSERT_NO_THROW(server.reset());
}

TEST_F(normal, connect_disconnect)
{
    init();
    ASSERT_NO_THROW(server->start());
    ASSERT_TRUE(server->accepting());

    add_socket();
    connect_sockets();

    ASSERT_NO_THROW(ASSERT_EQ(2, io_context.run_for(env::wait_dur_short)));

    ASSERT_EQ(1, proxy::get_clients(server).size());
    ASSERT_TRUE(server->accepting());

    close_sockets();

    ASSERT_NO_THROW(ASSERT_EQ(1, io_context.run_for(env::wait_dur_short)));

    ASSERT_EQ(0, proxy::get_clients(server).size());
    ASSERT_TRUE(server->accepting());
}

TEST_F(normal, write)
{
    init();
    server->start();
    add_socket();
    connect_sockets();

    io_context.run_for(env::wait_dur_short);

    sockets[0].send(asio::buffer("test1"));

    ASSERT_NO_THROW(ASSERT_EQ(1, io_context.run_for(env::wait_dur_short)));

    ASSERT_EQ(1, proxy::get_clients(server).size());

    ASSERT_TRUE(server->accepting());

    ASSERT_EQ(1, proxy::get_messages(proxy::get_clients(server).back()).size());
    ASSERT_TRUE(proxy::get_message_builder(proxy::get_clients(server).back()).str().empty());

    ASSERT_EQ("test1", proxy::get_messages(proxy::get_clients(server).back()).front());

    sockets[0].send(asio::buffer("test2\0test3"));

    ASSERT_NO_THROW(ASSERT_EQ(1, io_context.run_for(env::wait_dur_short)));

    ASSERT_EQ(3, proxy::get_messages(proxy::get_clients(server).back()).size());
    ASSERT_TRUE(proxy::get_message_builder(proxy::get_clients(server).back()).str().empty());

    ASSERT_EQ("test1", proxy::get_messages(proxy::get_clients(server).back()).front());
    proxy::get_messages(proxy::get_clients(server).back()).pop();
    ASSERT_EQ("test2", proxy::get_messages(proxy::get_clients(server).back()).front());
    proxy::get_messages(proxy::get_clients(server).back()).pop();
    ASSERT_EQ("test3", proxy::get_messages(proxy::get_clients(server).back()).front());
    proxy::get_messages(proxy::get_clients(server).back()).pop();

    sockets[0].send(asio::buffer({ 't', 'e', 's', 't', '4' }));

    ASSERT_NO_THROW(ASSERT_EQ(1, io_context.run_for(env::wait_dur_short)));

    ASSERT_TRUE(proxy::get_messages(proxy::get_clients(server).back()).empty());
    ASSERT_EQ("test4", proxy::get_message_builder(proxy::get_clients(server).back()).str());

    sockets[0].send(asio::buffer({ '\0', '\0', 't', 'e', 's', 't', '5', '\0', 't', 'e', 's', 't', '6' }));

    ASSERT_NO_THROW(ASSERT_EQ(1, io_context.run_for(env::wait_dur_short)));

    ASSERT_EQ(2, proxy::get_messages(proxy::get_clients(server).back()).size());

    ASSERT_EQ("test4", proxy::get_messages(proxy::get_clients(server).back()).front());
    proxy::get_messages(proxy::get_clients(server).back()).pop();

    ASSERT_EQ("test5", proxy::get_messages(proxy::get_clients(server).back()).front());
    proxy::get_messages(proxy::get_clients(server).back()).pop();

    ASSERT_EQ("test6", proxy::get_message_builder(proxy::get_clients(server).back()).str());

    sockets[0].send(asio::buffer("test7\0\0"));

    ASSERT_NO_THROW(ASSERT_EQ(1, io_context.run_for(env::wait_dur_short)));

    ASSERT_EQ(1, proxy::get_messages(proxy::get_clients(server).back()).size());
    ASSERT_TRUE(proxy::get_message_builder(proxy::get_clients(server).back()).str().empty());

    ASSERT_EQ("test6test7", proxy::get_messages(proxy::get_clients(server).back()).front());

    close_sockets();
}

}

int main(int argc, char** argv, char** env)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    testing::Environment* const foo_env = testing::AddGlobalTestEnvironment(new mmocli::env(env));
    return RUN_ALL_TESTS();
}
