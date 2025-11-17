#include "runtime/rt_log.h"

#include "platform/plat_api.h"

namespace
{
    void log_with_prefix(const char *prefix, const char *message)
    {
        if (message == 0)
        {
            message = "(null)";
        }

        char buffer[512];
        buffer[0] = '\0';

        unsigned int offset = 0u;
        while (prefix[offset] != '\0' && offset < sizeof(buffer) - 1u)
        {
            buffer[offset] = prefix[offset];
            ++offset;
        }

        if (offset < sizeof(buffer) - 2u)
        {
            buffer[offset++] = ' ';
        }

        unsigned int i = 0u;
        while (message[i] != '\0' && offset < sizeof(buffer) - 1u)
        {
            buffer[offset++] = message[i++];
        }
        buffer[offset] = '\0';

        plat_log_message(buffer);
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
