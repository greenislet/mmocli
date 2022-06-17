#include <cassert>

#include <curses.h>
#include <panel.h>

#include <limits>

#include "window.hpp"
#include "label.hpp"
#include "field.hpp"
#include "button.hpp"
#include "ezterm_p.hpp"
#include "window_impl.hpp"

namespace ezterm
{

window::window(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    : container(x, y)
    , impl_(new window_impl(::subwin(main_window->impl_->handler, height, width, y, x)))
{
    init();
}

window::window(window_impl* impl)
    : container(0, 0)
    , impl_(impl)
{
    init();
}

void window::init()
{
    getparyx(impl_->handler, y_, x_);
    if (x_ == std::numeric_limits<decltype(x_)>::max())
        x_ = 0;
    if (y_ == std::numeric_limits<decltype(y_)>::max())
        y_ = 0;
    getmaxyx(impl_->handler, height_, width_);

    keypad(true);
    box();
}

void window::show()
{
    for (widget* child : children_)
        child->show();
    ::wborder(impl_->handler, ACS_VLINE,
        ACS_VLINE,
        ACS_HLINE,
        ACS_HLINE,
        ACS_ULCORNER,
        ACS_URCORNER,
        ACS_LLCORNER,
        ACS_LRCORNER);
}

void window::hide()
{
    for (widget* child : children_)
        child->hide();
    ::wborder(impl_->handler, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
}

void window::box()
{
    ::box(impl_->handler, 0, 0);
}

bool window::keypad(bool val)
{
    return ::keypad(impl_->handler, val ? TRUE : FALSE) != ERR;
}

unsigned int window::width() const
{
    return width_;
}

unsigned int window::height() const
{
    return height_;
}

unsigned int window::x() const
{
    return x_;
}

unsigned int window::y() const
{
    return y_;
}

//window* window::add_window(unsigned int rel_x, unsigned int rel_y, unsigned int width, unsigned int height)
//{
//    window* sub_window = new window(x_ + rel_x + 1, y_ + rel_y + 1, width, height);
//    children_.push_back(sub_window);
//    return sub_window;
//}
//
//
//label* window::add_label(unsigned int rel_x, unsigned int rel_y, std::string const& text)
//{
//    label* lbl = new label(x_ + rel_x + 1, y_ + rel_y + 1, text);
//
//    children_.push_back(lbl);
//
//    return lbl;
//}
//
//field* window::add_field(unsigned int rel_x, unsigned int rel_y, unsigned int size, bool is_password)
//{
//    field* fld = new field(x_ + rel_x + 1, y_ + rel_y + 1, size, is_password);
//
//    children_.push_back(fld);
//
//    return fld;
//}
//
//button* window::add_button(unsigned int rel_x, unsigned int rel_y, std::string const& text, button::activate_callback && on_activate)
//{
//    button* btn = new button(x_ + rel_x + 1, y_ + rel_y + 1, text, std::move(on_activate));
//
//    children_.push_back(btn);
//
//    return btn;
//}

} // namespace ezterm
