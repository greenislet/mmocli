#include <chrono>

#include "server.hpp"
#include "log.hpp"
#include "tcp_server.hpp"

using namespace std::literals::chrono_literals;

void show_help()
{
    mmocli::_log("commands:", static_cast<std::ostream & (*)(std::ostream&)>(std::endl), "\thelp",
        static_cast<std::ostream& (*)(std::ostream&)>(std::endl), "\texit",
        static_cast<std::ostream& (*)(std::ostream&)>(std::endl));
}

void handle_user_input(mmocli::server & server)
{
    std::string input;
    for (;;)
    {
        std::cin >> input;
        if (input == "help")
        {
            std::lock_guard lg(mmocli::log_mutex);
            mmocli::_log("");
            show_help();
            mmocli::_log("");
        }
        else if (input == "exit")
        {
            server.stop();
            return;
        }
        else
        {
            std::lock_guard lg(mmocli::log_mutex);

            mmocli::_log("");
            mmocli::_log("Unknown command: ", input);
            mmocli::_log("");
            show_help();
            mmocli::_log("");
        }
        std::this_thread::sleep_for(100ms);
    }
}

int main(int argc, char** argv, char** env)
{
    mmocli::server server;

    server.start();

    handle_user_input(server);
}
