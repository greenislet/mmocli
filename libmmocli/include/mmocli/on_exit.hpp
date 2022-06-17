#pragma once

#include <concepts>

namespace mmocli
{

template<class T>
    requires std::invocable<T>
class on_exit
{
private:
    T callable_;

public:
    on_exit(T const& callable)
        : callable_(callable)
    {}

    on_exit(T&& callable)
        : callable_(std::move(callable))
    {}

    ~on_exit()
    {
        callable_();
    }
};

} // namespace mmocli
