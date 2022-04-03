#include <chrono>

#include "launch.hpp"
#include "log.hpp"
#include "tcp_server.hpp"

using namespace std::literals::chrono_literals;

void show_help()
{
    _MMOCLI_MY_LOG("commands:" << std::endl
        << "\thelp" << std::endl
        << "\texit" << std::endl
    );
}

void handle_user_input(std::shared_ptr<mmocli::tcp_server> server_p)
{
    std::string input;
    for (;;)
    {
        std::cin >> input;
        if (input == "help")
        {
            std::lock_guard lg(mmocli::log_mutex);
            _MMOCLI_MY_LOG("");
            show_help();
            _MMOCLI_MY_LOG("");
        }
        else if (input == "exit")
        {
            server_p->shutdown();
            return;
        }
        else
        {
             std::lock_guard lg(mmocli::log_mutex);
            _MMOCLI_MY_LOG("");
            _MMOCLI_MY_LOG("Unknown command: " << input);
            _MMOCLI_MY_LOG("");
            show_help();
            _MMOCLI_MY_LOG("");
        }
        std::this_thread::sleep_for(100ms);
    }
}

int main(int argc, char** argv, char** env)
{
    mmocli::resources resources = mmocli::launch();

    handle_user_input(resources.tcp_server_p);
}
