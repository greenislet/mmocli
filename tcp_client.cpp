#include "tcp_client.hpp"

#include "log.hpp"
#include "tcp_server.hpp"

using namespace std::placeholders;
using namespace boost;

namespace mmocli
{

tcp_client::tcp_client(boost::asio::io_context& io_context, asio::ip::tcp::socket&& socket, tcp_server& tcp_server)
    : addr_str_(socket.remote_endpoint().address().to_string() + ":" + std::to_string(socket.remote_endpoint().port()))
    , io_context_(io_context)
    , socket_(std::move(socket))
    , tcp_server_(tcp_server)
{}

void tcp_client::start()
{
    do_receive();
}

void tcp_client::do_receive()
{
    socket_.async_receive(asio::buffer(read_buffer_), std::bind(&tcp_client::on_receive, shared_from_this(), _1, _2));
}

std::vector<boost::system::error_code> critical_errors =
{
    boost::asio::error::eof
};

static bool is_critical(boost::system::error_code const& error)
{
    return std::find(critical_errors.cbegin(), critical_errors.cend(), error) == critical_errors.cend();
}

template<class T>
static std::string sanitize(T const& char_range, size_t cnt)
{
    std::string out;

    out.reserve(char_range.size());

    for (size_t i = 0; i < cnt; ++i)
        if (std::isprint(char_range[i]))
            out += char_range[i];
        else
            out += '\\' + std::to_string((int)char_range[i]);
    return out;
}

void tcp_client::on_receive(boost::system::error_code const& error, size_t bytes_transferred)
{
    if (error)
    {
        log("tcp client", addr_str_ + " on receive", is_critical(error) ? color::red : color::cyan) << error.value() << ":" << error.message() << endl;
        tcp_server_.remove_client(shared_from_this());
        return;
    };

    assert(bytes_transferred > 0);

    log("tcp client", addr_str_ + " on receive", color::cyan) << sanitize(read_buffer_, bytes_transferred) << endl;

    read_buffer::const_iterator begin = read_buffer_.cbegin();
    read_buffer::const_iterator end = read_buffer_.cbegin() + bytes_transferred;
    do {
        read_buffer::const_iterator it = std::find(begin, end, '\0');
        if (it == end)
        {
            message_builder_ << std::string(begin, end);
            break;
        }
        else
        {
            if (it > begin + 1 || (it == read_buffer_.cbegin() && !message_builder_.str().empty()))
            {
                message_builder_ << std::string(begin, it);
                {
                    std::lock_guard lock_messages(messages_mutex_);
                    messages_.emplace(message_builder_.str());
                }
                message_builder_.str("");
            }
            begin = it + 1;
        }
    } while (begin != end);

    do_receive();
}

} // namespace mmocli
