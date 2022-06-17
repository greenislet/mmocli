#include "container.hpp"

#include "interactive.hpp"

namespace ezterm
{

container::container(unsigned int x, unsigned int y)
    : widget(x, y)
{}

container::~container()
{
    for (widget* child : children_)
        delete child;
}

void container::add(widget* widget)
{
    children_.push_back(widget);
}

} // namespace ezterm
