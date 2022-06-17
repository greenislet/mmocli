#include <curses.h>

#include "button.hpp"
#include "ezterm_p.hpp"
#include "window.hpp"
#include "window_impl.hpp"

namespace ezterm
{

button::button(unsigned int x, unsigned int y, std::string const& text)
    : interactive(x, y)
    , text_(text)
    , on_activate_([]{})
{
    frame_impl_ = new window_impl(::subwin(main_window->impl_->handler, 3, text.size() + 2, y, x));
    ::box(frame_impl_->handler, 0, 0);

    show();
}

void button::on_activate(activate_callback const& on_activate)
{
    on_activate_ = on_activate;
}

static void print_space(unsigned int x, unsigned int y)
{
    print(x, y, " ");
}

void button::print_empty()
{
    for (unsigned int i = 0; i < text_.size(); ++i)
        print_space(x_ + 1 + i, y_ + 1);
}

void button::show()
{
    print_empty();
    if (focused_)
        print(x_ + 1, y_ + 1, text_, { color::black, color::white });
    else
        print(x_ + 1, y_ + 1, text_);
}

void button::hide()
{
    ::wborder(frame_impl_->handler, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    print_empty();
}

void button::focus()
{
    show();
}

void button::unfocus()
{
    show();
}

void button::input(int ch)
{
    if (ch == 10 || ch == 32)
        on_activate_();
}

} // namespace ezterm
