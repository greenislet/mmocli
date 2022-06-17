#pragma once

#include "config.hpp"

#include "interactive.hpp"

namespace ezterm
{

class EZTERM_API field : public interactive
{
private:
    unsigned int size_;
    unsigned int seek_;
    std::string  text_;
    bool         is_password_;

public:
    field(unsigned int x, unsigned int y, unsigned int size, bool is_password=false);

    virtual void show() override;
    virtual void hide() override;

    void print_empty();

    virtual void focus() override;
    virtual void unfocus() override;

    virtual void input(int ch) override;
};

} // namespace ezterm
