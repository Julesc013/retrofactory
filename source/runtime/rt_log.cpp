#include "runtime/rt_log.h"

#include <cstdio>

namespace
{
    RtLogSink g_sink = 0;

    void emit(const char *msg)
    {
        if (g_sink)
        {
            g_sink(msg);
        }
        else
        {
            std::printf("%s\n", msg);
        }
    }
}

void rt_log_init(RtLogSink sink)
{
    g_sink = sink;
}

void rt_log_info(const char *msg)
{
    emit(msg);
}

void rt_log_warn(const char *msg)
{
    emit(msg);
}

void rt_log_error(const char *msg)
{
    emit(msg);
}
