#pragma once

#include "config.hpp"

#include <vector>

#include "widget.hpp"
#include "interactive.hpp"
#include "container.hpp"

namespace ezterm
{

class interactive;

class EZTERM_API container : public widget
{
protected:
    std::vector<widget*> children_;

public:
    container(unsigned int x, unsigned int y);
    ~container();

public:
    void add(widget* widget);
};

} // namespace ezterm
