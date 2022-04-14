#include "log.hpp"

namespace mmocli
{

std::recursive_mutex log_mutex;
unsigned int port_max_size = 5;
unsigned int time_max_size = 8;
unsigned int title_max_size = 15;
unsigned int subtitle_max_size = 30;
unsigned int message_max_size = 50;
std::ostream* log_stream = &std::cout;
std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

std::map<color, std::string> colors = {
    { color::black  , std::string("30m") },
    { color::red    , std::string("31m") },
    { color::green  , std::string("32m") },
    { color::yellow , std::string("33m") },
    { color::blue   , std::string("34m") },
    { color::magenta, std::string("35m") },
    { color::cyan   , std::string("36m") },
    { color::white  , std::string("37m") },
    { color::reset  , std::string("0m") }
};

endl_t endl;

std::string log(std::string const& title, std::string const& subtitle, color color)
{
    std::ostringstream oss;
    oss << "\033[1;" << colors[color] << "["
        << std::left
        << std::setw(mmocli::port_max_size) << std::this_thread::get_id() << "] ["
        << std::setw(mmocli::time_max_size) << std::fixed << std::setprecision(8) << std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - mmocli::start_time).count() << "] ["
        << std::setw(mmocli::title_max_size) << title << "] ["
        << std::setw(mmocli::subtitle_max_size) << subtitle << "] "
        << "\033[1;" << colors[color];

    return oss.str();
}

std::ostream& operator<<(std::ostream& os, endl_t)
{
    os << "\033[1;" << colors[color::reset] << std::endl;
    return os;
}

}
