#include "log.hpp"

#ifdef _WIN32
# include <Windows.h>
#endif

#include <iostream>

namespace mmocli
{

std::recursive_mutex log_mutex;
unsigned int port_max_size = 5;
unsigned int time_max_size = 10;
unsigned int title_max_size = 15;
unsigned int subtitle_max_size = 25;
unsigned int message_max_size = 50;
std::ostream* log_stream = &std::cout;
std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

#if defined(_WIN32)
color reset_color = color::white;
#else
color reset_color = color::reset;
#endif


#if defined(_WIN32)
std::map<color, WORD> colors = {
    { color::dark_blue  , 1 },
    { color::green   , 2 },
    { color::cyan    , 3 },
    { color::red     , 4 },
    { color::magenta , 5 },
    { color::yellow  , 6 },
    { color::white   , 7 },
    { color::grey    , 8 },
    { color::blue    , 9 }
};
#else
std::map < color, std::string > colors = {
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
#endif

std::ostream& log(std::string const& title, std::string const& subtitle, color color)
{
    log_mutex.lock();

    try {
#if defined(_WIN32)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colors[color]);
#else
        * log_stream << "\033[1;" << colors[color];
#endif
            * log_stream << std::left << "["
            << std::setw(mmocli::port_max_size) << std::this_thread::get_id() << "] ["
            << std::setw(mmocli::time_max_size) << std::fixed << std::setprecision(8) << std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - mmocli::start_time).count() << "] ["
            << std::setw(mmocli::title_max_size) << title << "] ["
            << std::setw(mmocli::subtitle_max_size) << subtitle << "] ";
    }
    catch (std::exception const& e) {
        log_mutex.unlock();
        throw e;
    }
    return *log_stream;
}

std::ostream& log(color color)
{
    log_mutex.lock();

#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colors[color]);
#else
    * log_stream << "\033[1;" + colors[color];
#endif

    return *log_stream;
}

flush_t flush;

std::ostream& operator<<(std::ostream& os, flush_t)
{
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colors[reset_color]);
#else
    os << "\033[1;" << colors[reset_color];
#endif
    log_mutex.unlock();
    return os;
}

endl_t endl;

std::ostream& operator<<(std::ostream& os, endl_t)
{
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colors[reset_color]);
#else
    os << "\033[1;" << colors[reset_color];
#endif
    os << std::endl;
    log_mutex.unlock();
    return os;
}

}
