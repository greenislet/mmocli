#include <iostream>
#include <memory>
#include <thread>
#include <tuple>

#include <boost/asio/io_context.hpp>

#include "log.hpp"
#include "tcp_server.hpp"

namespace mmocli
{

std::tuple<std::shared_ptr<std::vector<std::jthread>>,
	std::shared_ptr<boost::asio::io_context>,
	std::shared_ptr<tcp_server>> launch()
{
	unsigned short  port = 2222;
	unsigned int    nb_threads = std::thread::hardware_concurrency();


	auto io_context_p = std::make_shared<boost::asio::io_context>();

	auto network_threads_p = std::make_shared<std::vector<std::jthread>>();

	auto tcp_server_p = std::make_shared<tcp_server>(*io_context_p, port);

	tcp_server_p->start();

	for (unsigned int i = 0; i < nb_threads; ++i)
	{
		network_threads_p->emplace_back([io_context_p, i]
			{
				MMOCLI_LOG("STARTUP", "THREAD #" << i + 1 << " RUNNING");
				try {
					io_context_p->run();
				}
				catch (std::exception const& e) {
					MMOCLI_LOG("ERROR", "Exception thrown : " << e.what());
				}
				MMOCLI_LOG("EXIT", "THREAD #" << i + 1 << " STOPPED");
			});
	}

	return { network_threads_p, io_context_p, tcp_server_p };
}

}
