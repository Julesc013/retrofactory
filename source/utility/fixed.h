#pragma once

#include "system/types.h"

typedef i32 fix32;

enum
{
    FIXED_SHIFT = 16,
    FIXED_ONE = 1 << FIXED_SHIFT
};

inline fix32 fixed_from_int(i32 value) { return value << FIXED_SHIFT; }
inline i32 fixed_to_int(fix32 value) { return value >> FIXED_SHIFT; }

inline fix32 fixed_from_float(float value)
{
    const i64 scaled = static_cast<i64>(value * static_cast<float>(FIXED_ONE));
    return static_cast<fix32>(scaled);
}

inline float fixed_to_float(fix32 value)
{
    return static_cast<float>(value) / static_cast<float>(FIXED_ONE);
}

inline fix32 fixed_mul(fix32 a, fix32 b)
{
    return static_cast<fix32>((static_cast<i64>(a) * static_cast<i64>(b)) >> FIXED_SHIFT);
}

inline fix32 fixed_div(fix32 a, fix32 b)
{
    if (b == 0)
    {
        return 0;
    }
    return static_cast<fix32>((static_cast<i64>(a) << FIXED_SHIFT) / static_cast<i64>(b));
}
