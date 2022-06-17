#include <iostream>

#include <ezterm/ezterm.hpp>
#include <ezterm/window.hpp>
#include <ezterm/label.hpp>
#include <ezterm/field.hpp>
#include <ezterm/button.hpp>

int main(int argc, char** argv, char** env)
{
    ezterm::window* mainwin = ezterm::init();

    unsigned int subwin_width = 50;
    unsigned int subwin_height = subwin_width / 4;
    unsigned int subwin_x = mainwin->width() / 2 - subwin_width / 2;
    unsigned int subwin_y = mainwin->height() / 2 - subwin_height / 2;

    //ezterm::window* subwin = mainwin->add_window(subwin_x, subwin_y, subwin_width, subwin_height);

    ezterm::window* subwin = mainwin->add<ezterm::window>(subwin_x, subwin_y, subwin_width, subwin_height);
    unsigned int align = 13;

    ezterm::label* email_label = subwin->add<ezterm::label>(1, 1, "email");
    ezterm::field* email_field = subwin->add<ezterm::field>(1 + email_label->size() +  (align - email_label->size() - 1), 1, 34);
    

    ezterm::label* pwd_label = subwin->add<ezterm::label>(1, 2, "password");
    ezterm::field* pwd_field = subwin->add<ezterm::field>(1 + pwd_label->size() + (align - pwd_label->size() - 1), 2, 34, true);

    std::string login_str("login");
    ezterm::button* login_btn = subwin->add<ezterm::button>(subwin->width() - login_str.size() - 4 - 1, 3, login_str);

    std::string forgotten_str("forgotten password");
    ezterm::button* forgotten_btn = subwin->add<ezterm::button>(1, 7, forgotten_str);

    std::string create_str("create account");
    ezterm::button* create_btn = subwin->add<ezterm::button>(subwin->width() - create_str.size() - 4 - 1, 7, create_str);

    create_btn->on_activate([=]
        {
            subwin->hide();
            //ezterm::window* register_win = mainwin->add<ezterm::window>(subwin_x, subwin_y, subwin_width, subwin_height);
            //ezterm::label* email1_label = register_win->add<ezterm::label>(1, 1, "email");
            //ezterm::field* email1_field = register_win->add<ezterm::field>(1 + email_label->size() + (align - email_label->size() - 1), 1, 34);
            //ezterm::label* email2_label = register_win->add<ezterm::label>(1, 3, "email");
            //ezterm::field* email2_field = register_win->add<ezterm::field>(1 + email_label->size() + (align - email_label->size() - 1), 3, 34);

        });
    email_field->begin_focus();

    ezterm::loop();
    ezterm::terminate();
}
