#pragma once

#include "config.hpp"

#include <memory>
#include <chrono>

#include <boost/asio.hpp>

using namespace std::literals::chrono_literals;

namespace mmocli
{

class MMOCLI_API game_server : public std::enable_shared_from_this<game_server>
{
public:
    using pointer                                      = std::shared_ptr<game_server>;
private:
    typedef std::chrono::duration<double, std::ratio<1>> delta_duration;
private:
    boost::asio::io_context&                             io_context_;
    boost::asio::steady_timer                            loop_timer_;
    std::chrono::steady_clock::time_point                wake_time_;
    std::chrono::steady_clock::time_point                start_time_;
    std::chrono::steady_clock::duration                  tick_duration_;
public:                                                  
                                                         game_server(boost::asio::io_context& io_context);
public:
    void                                                 start(std::chrono::steady_clock::duration const& tick_duration, std::chrono::steady_clock::time_point const& start_time);
    std::chrono::steady_clock::time_point                start(std::chrono::steady_clock::duration const& tick_duration = 250ms);
private:                                                 
    void                                                 cycle(boost::system::error_code const& error, unsigned int nb_ticks);
    void                                                 do_wait(unsigned int nb_ticks);
};

} // namespace mmocli