#include <stdio.h>

#include <gtest/gtest.h>

#include "tests.hpp"

TEST(game_server, test)
{

}

int main(int argc, char** argv, char** env)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    testing::Environment* const foo_env = testing::AddGlobalTestEnvironment(new mmocli::env(env));
    return RUN_ALL_TESTS();
}
