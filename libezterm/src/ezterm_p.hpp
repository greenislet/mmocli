#pragma once

#include <string>

#include "color.hpp"

namespace ezterm
{

class interactive;
class window;

void print(unsigned int x, unsigned int y, std::string const& text, std::pair<color, color> const& foreground_background = { color::white, color::black });

extern interactive* first_built_interactive;
extern interactive* last_built_interactive;
extern interactive* focused_interactive;

extern window* main_window;

}
