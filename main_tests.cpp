#include <thread>

#include <gtest/gtest.h>

#include "launch.hpp"

int main()
{
	std::jthread server_thread([]
		{
			mmocli::launch();
		});
}
