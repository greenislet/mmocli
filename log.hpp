#pragma once

#include "config.hpp"

#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

namespace mmocli
{

MMOCLI_API extern std::recursive_mutex log_mutex;
MMOCLI_API extern unsigned int title_max_size;
MMOCLI_API extern unsigned int subtitle_max_size;
MMOCLI_API extern std::ostream *log_stream;

}

#if defined(MMOCLI_NO_LOG)
# define _MMOCLI_MY_LOG(to_log)
#else
# define _MMOCLI_MY_LOG(to_log)\
do {\
    std::lock_guard _my_lock(mmocli::log_mutex);\
    *mmocli::log_stream << to_log << std::endl;\
} while (0)
#endif

#define MMOCLI_LOG(title, subtitle, msg) _MMOCLI_MY_LOG("[" << std::setw(5) << std::this_thread::get_id() << "] [" << std::setw(mmocli::title_max_size) << title << "] [" << std::setw(mmocli::subtitle_max_size) << subtitle << "] " << msg)

#define MMOCLI_BLACK "30m"
#define MMOCLI_RED "31m"
#define MMOCLI_GREEN "32m"
#define MMOCLI_YELLOW "33m"
#define MMOCLI_BLUE "34m"
#define MMOCLI_MAGENTA "35m"
#define MMOCLI_CYAN "36m"
#define MMOCLI_WHITE "37m"

#define MMOCLI_RESET "\033[0m"

#define MMOCLI_LOG_COLOR(color, title, subtitle, msg) _MMOCLI_MY_LOG("\033[1;" << color << "[" << std::setw(5) << std::this_thread::get_id() << "] [" << std::setw(mmocli::title_max_size) << title << "] [" << std::setw(mmocli::subtitle_max_size) << subtitle << "] " << msg << MMOCLI_RESET)

#define MMOCLI_LOG_RED(t, s, m) MMOCLI_LOG_COLOR(MMOCLI_RED, t, s, m)

#define MMOCLI_ERROR(t, s, m) MMOCLI_LOG_RED(t, s, m)

/**
/foreground background
black        30         40
red          31         41
green        32         42
yellow       33         43
blue         34         44
magenta      35         45
cyan         36         46
white        37         47

reset             0  (everything back to normal)
bold/bright       1  (often a brighter shade of the same colour)
underline         4
inverse           7  (swap foreground and background colours)
bold/bright off  21
underline off    24
inverse off      27
**/
