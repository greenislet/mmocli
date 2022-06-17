#pragma once

#include <gtest/gtest.h>

#include <vector>
#include <sstream>

#include <mmocli/on_exit.hpp>
#include <mmocli/server.hpp>
#include <mmocli/game_server.hpp>
#include <mmocli/tcp_server.hpp>
#include <mmocli/tcp_client.hpp>
#include <mmocli/log.hpp>

namespace mmocli
{

class proxy
{
public:
    static tcp_server::clients_container& get_clients(tcp_server::pointer tcp_server);
    static tcp_client::message_container& get_messages(tcp_client::pointer tcp_client);
    static tcp_client::string_builder& get_message_builder(tcp_client::pointer tcp_client);
    static boost::asio::ip::tcp::socket& get_socket(tcp_client::pointer tcp_client);
};

}

namespace mmocli_tests
{

class env : public ::testing::Environment
{
private:
    char** env_;
public:
    static std::chrono::steady_clock::duration wait_dur_short;
    static std::chrono::steady_clock::duration wait_dur_long;
public:
    env(char** env);
public:
    void                                       SetUp() override;
    void                                       TearDown() override;
};

class test_tcp_server : public ::testing::Test
{
public:
    boost::asio::io_context                      io_context;
    std::vector<std::future<void>>               futures;
    mmocli::tcp_server::pointer                  server;
    boost::asio::ip::tcp::resolver::results_type results;
    std::vector<boost::asio::ip::tcp::socket>    sockets;

    void                                         init();
    void                                         add_sockets(unsigned int n);
    void                                         add_socket();
    void                                         connect_sockets();
    void                                         close_sockets();
    void                                         clear_sockets();
};

}
