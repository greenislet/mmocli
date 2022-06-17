#include <stdio.h>

#include <chrono>

#include <gtest/gtest.h>

#include "tests.hpp"

using namespace std::literals::chrono_literals;

TEST(test_game_server, instanciation_destruction)
{
    boost::asio::io_context io_context;
    std::shared_ptr<mmocli::game_server> server;
    ASSERT_NO_THROW(server = std::make_shared<mmocli::game_server>(io_context));
    ASSERT_NO_THROW(server.reset());
}

TEST(test_game_server, cycles)
{
    boost::asio::io_context io_context;
    auto server = std::make_shared<mmocli::game_server>(io_context);

    auto start_time = std::chrono::steady_clock::now() + 500ms;

    server->start(100ms, start_time);

    std::this_thread::sleep_until(start_time - 10ms);

    size_t n;
    ASSERT_NO_THROW(n = io_context.run_until(start_time + 500ms + 10ms));
    ASSERT_GT(n, 4);
    ASSERT_LT(n, 7);
}

int main(int argc, char** argv, char** env)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new mmocli_tests::env(env));
    return RUN_ALL_TESTS();
}
