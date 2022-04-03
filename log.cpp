#include "log.hpp"

namespace mmocli
{

std::recursive_mutex log_mutex;
unsigned int title_max_size = 7;
unsigned int subtitle_max_size = 28;
std::ostream *log_stream = &std::cout;

}
