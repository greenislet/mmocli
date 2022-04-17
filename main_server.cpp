#include <chrono>

#include <iostream>

#include "server.hpp"
#include "log.hpp"
#include "tcp_server.hpp"

using namespace std::literals::chrono_literals;

void show_help()
{
    mmocli::log() << "commands:" << mmocli::endl;
    mmocli::log() << "\thelp" << mmocli::endl;
    mmocli::log() << "\texit" << mmocli::endl;
}

void handle_user_input(mmocli::server& server)
{
    std::string input;
    for (;;)
    {
        std::cin >> input;
        if (input == "help")
        {
            std::lock_guard lg(mmocli::log_mutex);
            mmocli::log() << mmocli::endl;
            show_help();
            mmocli::log() << mmocli::endl;
        }
        else if (input == "exit")
        {
            return;
        }
        else
        {
            std::lock_guard lg(mmocli::log_mutex);

            mmocli::log() << mmocli::endl;
            mmocli::log() << "Unknown command: " << input;
            mmocli::log() << mmocli::endl;
            show_help();
            mmocli::log() << mmocli::endl;
        }
        std::this_thread::sleep_for(100ms);
    }
}

int main(int argc, char** argv, char** env)
{
    boost::asio::io_context io_context;

    mmocli::server server(io_context);

    server.start();

    server.launch();

    handle_user_input(server);

    io_context.stop();

    server.wait();
}
