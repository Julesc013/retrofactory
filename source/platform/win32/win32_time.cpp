#include <ctime>
#include "system/types.h"

u64 win32_time_now_ms()
{
    return static_cast<u64>((std::clock() * 1000) / CLOCKS_PER_SEC);
}

void win32_sleep_ms(u32 ms)
{
    const clock_t start = std::clock();
    const clock_t target = start + static_cast<clock_t>((ms * CLOCKS_PER_SEC) / 1000u);
    while (std::clock() < target)
    {
        /* busy wait; deterministic placeholder */
    }
}
