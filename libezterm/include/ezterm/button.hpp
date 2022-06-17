#pragma once

#include "config.hpp"

#include <string>
#include <memory>
#include <functional>

#include "interactive.hpp"

namespace ezterm
{

struct window_impl;

class EZTERM_API button : public interactive
{
public:
    using activate_proto = void();
    using activate_callback = std::function<activate_proto>;

private:
    std::string                    text_;
    window_impl*                   frame_impl_;
    activate_callback              on_activate_;

public:
    button(unsigned int x, unsigned int y, std::string const& text);

    void on_activate(activate_callback const& on_activate);
    void print_empty();

    virtual void show() override;
    virtual void hide() override;

    virtual void focus() override;
    virtual void unfocus() override;
    virtual void input(int ch) override;
};

}
