#include "interactive.hpp"
#include "ezterm_p.hpp"

namespace ezterm
{

interactive::interactive(unsigned int x, unsigned int y)
    : widget(x, y)
    , focused_(false)
    , next_(nullptr)
    , prev_(nullptr)
{
    if (first_built_interactive == nullptr)
        first_built_interactive = this;
    if (last_built_interactive != nullptr)
    {
        last_built_interactive->next_ = this;
        prev_ = last_built_interactive;
    }

    first_built_interactive->prev_ = this;
    next_ = first_built_interactive;
    last_built_interactive = this;
}

bool interactive::focused() const
{
    return focused_;
}

void interactive::begin_focus()
{
    focused_ = true;
    focused_interactive = this;
    focus();
}

void interactive::begin_unfocus()
{
    focused_ = false;
    unfocus();
}

} // namespace ezterm
