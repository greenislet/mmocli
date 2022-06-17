#pragma once

#include "config.hpp"

#include "widget.hpp"
#include "ezterm.hpp"

namespace ezterm
{

class EZTERM_API interactive : public widget
{
    friend EZTERM_API void loop();
protected:
    bool         focused_;
    interactive* next_;
    interactive* prev_;

public:
    interactive(unsigned int x, unsigned int y);

    bool         focused() const;
    void         begin_focus();
    void         begin_unfocus();
    virtual void focus() = 0;
    virtual void unfocus() = 0;
    virtual void input(int ch) = 0;
};

} // namespace ezterm
