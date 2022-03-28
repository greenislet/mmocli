#include "launch.hpp"
#include "log.hpp"
#include "tcp_server.hpp"

void show_help()
{
    _MMOCLI_MY_LOG("commands:" << std::endl
        << "\thelp" << std::endl
        << "\tinfo" << std::endl
        << "\texit" << std::endl
        //<< "\tio: set/unset io log: " << mmocli::io_log << std::endl
        //<< "\tdata: set/unset data log (no effect if io log is unset): " << mmocli::data_log
    );
}

void handle_user_input(std::shared_ptr<mmocli::tcp_server> server_p)
{
    std::string input;
    for (;;)
    {
        std::cin >> input;
        if (input == "info")
        {
            std::lock_guard lg(mmocli::log_mutex);
            //MMOCLI_LOG("INFO", "Connections: " << server_p->get_connections().size());
            //MMOCLI_LOG("INFO", "Entities: " << server_p->get_entities().size());
            /*LOG("INFO", "Threads: " << network_threads_.size() << " asio thread"
            << (network_threads_.size() > 1 ? "s" : "") << " + user input thread + game loop thread");*/
        }
        else if (input == "help")
        {
            std::lock_guard lg(mmocli::log_mutex);
            _MMOCLI_MY_LOG("");
            show_help();
            _MMOCLI_MY_LOG("");
        }
        //else if (input == "data")
        //    mmocli::data_log = !mmocli::data_log;
        //else if (input == "io")
        //    mmocli::io_log = !mmocli::io_log;
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
    }
}

int main(int argc, char** argv, char** env)
{
    std::tuple<std::shared_ptr<std::vector<std::jthread>>,
        std::shared_ptr<boost::asio::io_context>,
        std::shared_ptr<mmocli::tcp_server>> resources = mmocli::launch();

    handle_user_input(std::get<2>(resources));
}
