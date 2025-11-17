#include "runtime/rt_log.h"

#include <cstdio>

namespace
{
    void log_with_prefix(const char *prefix, const char *message)
    {
        if (message == 0)
        {
            message = "(null)";
        }
        std::fprintf(stderr, "%s %s\n", prefix, message);
    }
}

void log_info(const char *message)
{
    log_with_prefix("[INFO]", message);
}

void log_warn(const char *message)
{
    log_with_prefix("[WARN]", message);
}

void log_error(const char *message)
{
    log_with_prefix("[ERROR]", message);
}
