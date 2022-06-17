#include <ezterm/ezterm.hpp>
#include <ezterm/window.hpp>
#include <ezterm/field.hpp>

int main(int argc, char** argv, char** env)
{
    ezterm::window* mainwin = ezterm::init();

    ezterm::loop();
}
