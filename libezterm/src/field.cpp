#include <locale>

#include <curses.h>

#include "ezterm.hpp"
#include "ezterm_p.hpp"
#include "field.hpp"

namespace ezterm
{

field::field(unsigned int x, unsigned int y, unsigned int size, bool is_password)
    : interactive(x, y)
    , size_(size)
    , seek_(0)
    , is_password_(is_password)
{
    text_.reserve(size);
    show();
}

static void print_underscore(unsigned int x, unsigned int y, bool focused)
{
    if (focused)
        print(x, y, "_", { color::black, color::white });
    else
        print(x, y, "_");
}

void field::print_empty()
{
    for (unsigned int i = 0; i < size_; ++i)
        print_underscore(x_ + i, y_, focused_);
}

void field::show()
{
    std::string str;
    if (is_password_)
        str = std::string(seek_, '*');
    else
        str = text_;

    print_empty();
    if (focused_)
        print(x_, y_, str, { color::black, color::white });
    else
        print(x_, y_, str);
    if (seek_ == size_)
        ::move(y_, x_ + size_ - 1);
}

void field::hide()
{
    for (unsigned int i = 0; i < size_; ++i)
        print(x_ + i, y_, " ");
}

void field::focus()
{
    show();
    ::move(y_, x_ + seek_);
}

void field::unfocus()
{
    show();
}

void field::input(int ch)
{
    std::locale loc("C");

    if (ch == 8 && seek_ > 0)
    {
        text_.pop_back();
        --seek_;
    }
    else if (!std::isprint(ch, loc))
        return;
    else if (seek_ < size_)
    {
        text_ += static_cast<char>(ch);
        ++seek_;
    }

    show();
}

} // namespace ezterm
