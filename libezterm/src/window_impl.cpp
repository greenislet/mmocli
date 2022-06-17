#include "window_impl.hpp"

namespace ezterm
{

window_impl::window_impl(WINDOW* curses_handler)
    : handler(curses_handler)
{}

window_impl::~window_impl()
{
    ::delwin(handler);
}

}
