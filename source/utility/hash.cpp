#include "utility/hash.h"

#include <cstring>

u32 hash_bytes32(const void *data, u32 size)
{
    const u8 *bytes = static_cast<const u8 *>(data);
    u32 hash = 2166136261u;
    u32 i;
    for (i = 0u; i < size; ++i)
    {
        hash ^= bytes[i];
        hash *= 16777619u;
    }
    return hash;
}

u64 hash_bytes64(const void *data, u32 size)
{
    const u8 *bytes = static_cast<const u8 *>(data);
    u64 hash = 1469598103934665603ULL;
    u32 i;
    for (i = 0u; i < size; ++i)
    {
        hash ^= static_cast<u64>(bytes[i]);
        hash *= 1099511628211ULL;
    }
    return hash;
}

u32 hash_string32(const char *str)
{
    if (str == 0)
    {
        return 0u;
    }
    return hash_bytes32(str, static_cast<u32>(std::strlen(str)));
}

u32 hash_combine32(u32 a, u32 b)
{
    /* Deterministic mix of two values. */
    a ^= b + 0x9e3779b9u + (a << 6) + (a >> 2);
    return a;
}
