#include <ctime>
#include "system/types.h"

u64 posix_time_now_ms()
{
    return static_cast<u64>((std::clock() * 1000) / CLOCKS_PER_SEC);
}
