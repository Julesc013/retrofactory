#include "save/state_hash.h"

namespace
{
    uint64 mix64(uint64 value)
    {
        value ^= value >> 33;
        value *= 0xff51afd7ed558ccdULL;
        value ^= value >> 33;
        value *= 0xc4ceb9fe1a85ec53ULL;
        value ^= value >> 33;
        return value;
    }
}

uint64 compute_state_hash(const CoreState &state)
{
    uint64 combined = state.tick;
    combined ^= state.rng.state + 0x9e3779b97f4a7c15ULL + (combined << 6) + (combined >> 2);
    return mix64(combined);
}
