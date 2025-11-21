#pragma once

#include "system/types.h"

/* Canonical time unit: 1 tick = 1/60 second */
#define RF_TICKS_PER_SECOND 60u

static inline Tick ticks_from_seconds(u32 seconds)
{
    return static_cast<Tick>(seconds * RF_TICKS_PER_SECOND);
}

static inline u32 seconds_from_ticks(Tick ticks)
{
    return ticks / RF_TICKS_PER_SECOND;
}
