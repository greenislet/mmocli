#include "label.hpp"
#include "ezterm_p.hpp"

namespace ezterm
{

label::label(unsigned int x, unsigned int y, std::string const& text)
    : widget(x, y)
    , text_(text)
{
    show();
}

void label::show()
{
    print(x_, y_, text_);
}

void label::hide()
{
    print(x_, y_, std::string(text_.size(), ' '));
}

unsigned int label::size() const
{
    return text_.size();
}

} // namespace ezterm
