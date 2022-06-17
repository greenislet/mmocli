#include <string>
#include <map>

#include <curses.h>

#include "color.hpp"
#include "interactive.hpp"

namespace ezterm
{

window* main_window = nullptr;

interactive* first_built_interactive = nullptr;
interactive* last_built_interactive = nullptr;
interactive* focused_interactive = nullptr;

std::map<color, int> colors =
{
    {color::white, COLOR_WHITE},
    {color::black, COLOR_BLACK}
};

std::map<std::string, short> color_pairs;

short current_pair_idx = 1;

void print(unsigned int x, unsigned int y, std::string const& text, std::pair<color, color> const& foreground_background)
{
    std::string hash = std::to_string(static_cast<int>(foreground_background.first)) + std::to_string(static_cast<int>(foreground_background.second));

    short pair_idx;

    if (color_pairs.contains(hash))
        pair_idx = color_pairs[hash];
    else
    {
        pair_idx = current_pair_idx;
        color_pairs[hash] = current_pair_idx;
        ++current_pair_idx;
    }

    ::init_pair(pair_idx, colors[foreground_background.first], colors[foreground_background.second]);
    ::attron(COLOR_PAIR(pair_idx));
    ::mvprintw(y, x, "%s", text.data());
    ::attroff(COLOR_PAIR(pair_idx));
}

} // namespace ezterm
