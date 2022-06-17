#pragma once

#include <curses.h>

namespace ezterm
{

struct window_impl
{
    WINDOW* handler;

    window_impl(WINDOW* curses_handler);
    ~window_impl();
};

}
