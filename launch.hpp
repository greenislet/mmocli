#include <tuple>
#include <vector>
#include <thread>

#include <boost/asio/io_context.hpp>

#include "tcp_server.hpp"

namespace mmocli
{
std::tuple<std::shared_ptr<std::vector<std::jthread>>,
	std::shared_ptr<boost::asio::io_context>,
	std::shared_ptr<tcp_server>> launch();
}
