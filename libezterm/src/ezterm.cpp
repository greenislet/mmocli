#include "ezterm.hpp"

#include <cassert>

#include <curses.h>
#include <panel.h>

#include <map>
#include <stdexcept>
#include <string>
#include <memory>

#include "color.hpp"
#include "window.hpp"
#include "ezterm_p.hpp"

#include "window_impl.hpp"

namespace ezterm
{

static bool no_echo()
{
    return ::noecho() == OK;
}

static bool start_color()
{
    return ::start_color() == OK;
}

static void refresh()
{
    ::refresh();
}

window* init()
{
    WINDOW* main_window_impl = ::initscr();

    if (main_window_impl == nullptr)
        throw std::runtime_error("Could not initialize screen");

    no_echo();
    start_color();

    main_window = new window(new window_impl(main_window_impl));
    return main_window;
}

void terminate()
{
    delete main_window;
    main_window = nullptr;
    ::endwin();
}

void loop()
{
    int ch;

    if (focused_interactive == nullptr && first_built_interactive != nullptr)
        first_built_interactive->begin_focus();

    for (;;)
    {
        ::touchwin(main_window->impl_->handler);
        ::doupdate();
        ch = ::wgetch(main_window->impl_->handler);
        if (ch == 27)
            break;
        if (focused_interactive != nullptr)
        {
            if (ch == 9)
            {
                focused_interactive->begin_unfocus();
                if (focused_interactive->next_ != nullptr)
                    focused_interactive->next_->begin_focus();
                else
                    first_built_interactive->begin_focus();
            }
            else if (ch == 351)
            {
                focused_interactive->begin_unfocus();
                if (focused_interactive->prev_ != nullptr)
                    focused_interactive->prev_->begin_focus();
                else
                    last_built_interactive->begin_focus();
            }
            else
            {
                focused_interactive->input(ch);
            }
        }
    }
}

} // namespace ezterm
