#pragma once

#include "config.hpp"

namespace ezterm
{

class EZTERM_API widget
{
protected:
    unsigned int x_;
    unsigned int y_;

public:
    widget(unsigned int x, unsigned int y);
    virtual ~widget();

public:
    virtual void show() = 0;
    virtual void hide() = 0;
};

} // namespace ezterm
