#pragma once

#include <string>

#include "config.hpp"

#include "widget.hpp"

namespace ezterm
{

class EZTERM_API label : public widget
{
private:
    std::string text_;

public:
    label(unsigned int x, unsigned int y, std::string const& text);

    virtual void show() override;
    virtual void hide() override;

    unsigned int size() const;
};

} // namespace ezterm
