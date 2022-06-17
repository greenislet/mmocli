#pragma once

#include "config.hpp"

#include <memory>
#include <string>

#include "button.hpp"
#include "container.hpp"

namespace ezterm
{

struct window_impl;

class field;
class label;

class EZTERM_API window : public container
{
    friend class button;
    friend EZTERM_API window* init();
    friend EZTERM_API void loop();

private:
    window_impl* impl_;
    unsigned int width_;
    unsigned int height_;

private:
    window(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    window(window_impl* impl);

private:
    void init();

public:
    virtual void show() override;
    virtual void hide() override;

    void         box();
    bool         keypad(bool val);
    unsigned int width() const;
    unsigned int height() const;
    unsigned int x() const;
    unsigned int y() const;

    template<class T, class... Args>
    T* add(unsigned int rel_x, unsigned int rel_y, Args&&... args)
    {
        T* widget = new T(x_ + rel_x + 1, y_ + rel_y + 1, std::forward<Args>(args)...);

        children_.push_back(widget);

        return widget;
    }

    //window*      add_window(unsigned int x, unsigned int y, unsigned int widht, unsigned int height);
    //label*       add_label(unsigned int rel_x, unsigned int rel_y, std::string const& text);
    //field*       add_field(unsigned int rel_x, unsigned int rel_y, unsigned int size, bool is_password=false);
    //button*      add_button(unsigned int rel_x, unsigned int rel_y, std::string const& text, button::activate_callback&& on_activate);
};

} // namespace ezterm
