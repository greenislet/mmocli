#pragma once

#include "config.hpp"

#ifdef _WIN32
# include <Windows.h>
#endif

#include <iomanip>
#include <mutex>
#include <ostream>
#include <thread>
#include <map>
#include <functional>
#include <ostream>

namespace mmocli
{

MMOCLI_API extern                                       std::recursive_mutex log_mutex;
MMOCLI_API extern unsigned int                          port_max_size;
MMOCLI_API extern unsigned int                          time_max_size;
MMOCLI_API extern unsigned int                          title_max_size;
MMOCLI_API extern unsigned int                          subtitle_max_size;
MMOCLI_API extern unsigned int                          message_max_size;
MMOCLI_API extern std::ostream*                         log_stream;
MMOCLI_API extern std::chrono::steady_clock::time_point start_time;

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

enum class color
{
#if defined(_WIN32)
    dark_blue,
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    grey,
    white,
#else
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white,
    reset
#endif
};

MMOCLI_API extern color reset_color;

#if defined(_WIN32)
MMOCLI_API extern std::map<color, WORD> colors;
#else
MMOCLI_API extern std::map<color, std::string> colors;
#endif

MMOCLI_API std::ostream &log(std::string const& title, std::string const& subtitle, color color = reset_color);
MMOCLI_API std::ostream &log(color color = reset_color);

class MMOCLI_API flush_t
{};

class MMOCLI_API endl_t
{};

MMOCLI_API extern flush_t flush;
MMOCLI_API extern endl_t endl;

MMOCLI_API std::ostream& operator<<(std::ostream& os, flush_t);
MMOCLI_API std::ostream& operator<<(std::ostream& os, endl_t);

}
