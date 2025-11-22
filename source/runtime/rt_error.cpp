#include "runtime/rt_error.h"

const char *rt_error_to_string(RtErrorCode code)
{
    switch (code)
    {
    case RT_OK: return "ok";
    case RT_ERR_IO: return "io_error";
    case RT_ERR_INVALID: return "invalid";
    default: return "unknown";
    }
}
