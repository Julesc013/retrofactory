#include "runtime/rt_error.h"

#include <cstdlib>

#include "runtime/rt_log.h"

void report_fatal_error(const char *message)
{
    log_error(message);
    std::abort();
}
