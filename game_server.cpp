#include "game_server.hpp"

#include "log.hpp"

using namespace std::placeholders;
using namespace std::literals::chrono_literals;
using namespace boost;

namespace mmocli
{

game_server::game_server(asio::io_context& io_context)
    : io_context_(io_context)
    , loop_timer_(io_context_)
{}

void game_server::start(std::chrono::steady_clock::duration const& tick_duration)
{
    tick_duration_ = tick_duration;
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::steady_clock::duration elapsed = now - start_time;
    elapsed -= std::chrono::floor<std::chrono::seconds>(elapsed);
    wake_time_ = now + (1s - elapsed);

    log("game server", "startup", color::magenta)
        << "first cycle in " << std::chrono::duration_cast<std::chrono::duration<float>>(wake_time_ - std::chrono::steady_clock::now()).count()
        << " with tick duration of " << std::chrono::duration_cast<std::chrono::duration<float>>(tick_duration_).count() << endl;

    loop_timer_.expires_at(wake_time_);
    do_wait(0);
}

void game_server::cycle(boost::system::error_code const& error, unsigned int nb_ticks)
{

    if (error)
    {
        if (error.value() != 995)
            log("game server", "loop", color::red) << error.message() << endl;
        return;
    }

    //log(color::magenta, "game server", "loop");

    double delta = (std::chrono::duration_cast<delta_duration>(tick_duration_) * nb_ticks).count();


    // Prepare next call
    nb_ticks = 0;
    while (wake_time_ < std::chrono::steady_clock::now())
    {
        wake_time_ += tick_duration_;
        ++nb_ticks;
    }
    assert(nb_ticks >= 1);

    if (nb_ticks > 1)
        log("game server", "loop", color::magenta) << "retard of " << nb_ticks - 1 << " tick" << (nb_ticks - 1 > 1 ? "s" : "") << endl;

    //log(color::magenta, "game server", "loop", "next cycle at ", std::chrono::duration_cast<std::chrono::duration<float>>(wake_time_ - start_time).count());
    loop_timer_.expires_at(wake_time_);
    do_wait(nb_ticks);
}

void game_server::do_wait(unsigned int nb_ticks)
{
    loop_timer_.async_wait(std::bind(&game_server::cycle, shared_from_this(), _1, nb_ticks));
}

} // namespace mmocli
