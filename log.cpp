#include "log.hpp"

namespace mmocli
{

std::recursive_mutex log_mutex;
unsigned int title_max_size = 50;
std::ostream *log_stream = &std::cout;

}
