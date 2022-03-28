#pragma once

#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

namespace mmocli
{

extern std::recursive_mutex log_mutex;
extern unsigned int title_max_size;
extern std::ostream *log_stream;

}

#define _MMOCLI_MY_LOG(to_log)\
do {\
	std::lock_guard _my_lock(mmocli::log_mutex);\
	*mmocli::log_stream << to_log << std::endl;\
} while (0)

#define MMOCLI_LOG(title, to_log) _MMOCLI_MY_LOG("[" << std::setw(5) << std::this_thread::get_id() << "] [" << std::setw(mmocli::title_max_size) << title << "] " << to_log)
