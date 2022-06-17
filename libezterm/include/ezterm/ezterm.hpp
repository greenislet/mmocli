#pragma once

#include "config.hpp"

namespace ezterm
{

class window;

EZTERM_API window* init();
EZTERM_API void terminate();

EZTERM_API void loop();

};
