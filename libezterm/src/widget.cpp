#include "widget.hpp"

namespace ezterm
{

widget::widget(unsigned int x, unsigned int y)
    : x_(x)
    , y_(y)
    , shown_(true)
{}

widget::~widget()
{}

bool widget::shown()
{
    return shown_;
}

void widget::begin_show()
{
    shown_ = true;
    show();
}

void widget::begin_hide()
{
    shown_ = false;
    hide();
}

} // namespace ezterm
