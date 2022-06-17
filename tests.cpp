#include "tests.hpp"

using namespace boost;

namespace mmocli
{

tcp_server::clients_container& proxy::get_clients(tcp_server::pointer tcp_server)
{
    return tcp_server->clients_;
}

tcp_client::message_container& proxy::get_messages(tcp_client::pointer tcp_client)
{
    return tcp_client->messages_;
}

tcp_client::string_builder& proxy::get_message_builder(tcp_client::pointer tcp_client)
{
    return tcp_client->message_builder_;
}

boost::asio::ip::tcp::socket& proxy::get_socket(tcp_client::pointer tcp_client)
{
    return tcp_client->socket_;
}

}

namespace mmocli_tests
{

std::chrono::steady_clock::duration env::wait_dur_short = std::chrono::duration_cast<std::chrono::steady_clock::duration>(0.02s);
std::chrono::steady_clock::duration env::wait_dur_long = std::chrono::duration_cast<std::chrono::steady_clock::duration>(1s);

env::env(char** env)
    : env_(env)
{}

void env::SetUp()
{
    for (int i = 0; env_[i] != nullptr; ++i)
        if (env_[i] == std::string("MMOCLI_LOG=FALSE"))
        {
            mmocli::log_stream = new std::ostringstream;
            break;
        }
        else if (env_[i] == std::string("MMOCLI_LOG=TRUE"))
        {
            mmocli::log_stream = &std::cout;
            int factor = 2;
            wait_dur_short *= factor;
            wait_dur_long *= factor;
            break;
        }
}

void env::TearDown()
{
    for (int i = 0; env_[i] != nullptr; ++i)
        if (env_[i] == std::string("MMOCLI_LOG=FALSE"))
        {
            delete mmocli::log_stream;
            mmocli::log_stream = nullptr;
            break;
        }
}

void test_tcp_server::init()
{
    server = std::make_shared<mmocli::tcp_server>(2222, io_context);

    asio::ip::tcp::resolver resolver(io_context);

    results = resolver.resolve(asio::ip::tcp::resolver::query("127.0.0.1", "2222"));
}

void test_tcp_server::add_sockets(unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        add_socket();
}

void test_tcp_server::add_socket()
{
    sockets.emplace_back(io_context);
}

void test_tcp_server::connect_sockets()
{
    for (asio::ip::tcp::socket& socket : sockets)
        socket.async_connect(*results, [](boost::system::error_code const& error)
            {
                ASSERT_FALSE(error);
            });
}

void test_tcp_server::close_sockets()
{
    for (asio::ip::tcp::socket& socket : sockets)
        ASSERT_NO_THROW(socket.close());
}

void test_tcp_server::clear_sockets()
{
    sockets.clear();
}

}
