#pragma once

#include "system/types.h"
#include <new>

struct Bitset
{
    u32 *words;
    u32 bit_count;
    u32 word_count;

    Bitset() : words(0), bit_count(0u), word_count(0u) {}
};

bool bitset_init(Bitset *set, u32 bits);
void bitset_free(Bitset *set);
void bitset_clear(Bitset *set);
bool bitset_set(Bitset *set, u32 index, bool value);
bool bitset_test(const Bitset *set, u32 index);

inline static u32 bitset_word_index(u32 bit_index)
{
    return bit_index >> 5;
}

inline static u32 bitset_mask(u32 bit_index)
{
    return 1u << (bit_index & 31u);
}

inline bool bitset_init(Bitset *set, u32 bits)
{
    if (set == 0)
    {
        return false;
    }
    bitset_free(set);

    set->bit_count = bits;
    set->word_count = (bits + 31u) / 32u;
    if (set->word_count == 0u)
    {
        return true;
    }

    set->words = new (std::nothrow) u32[set->word_count];
    if (set->words == 0)
    {
        set->bit_count = 0u;
        set->word_count = 0u;
        return false;
    }
    bitset_clear(set);
    return true;
}

inline void bitset_free(Bitset *set)
{
    if (set == 0)
    {
        return;
    }
    if (set->words != 0)
    {
        delete[] set->words;
        set->words = 0;
    }
    set->bit_count = 0u;
    set->word_count = 0u;
}

inline void bitset_clear(Bitset *set)
{
    if (set == 0 || set->words == 0)
    {
        return;
    }
    u32 i;
    for (i = 0u; i < set->word_count; ++i)
    {
        set->words[i] = 0u;
    }
}

inline bool bitset_set(Bitset *set, u32 index, bool value)
{
    if (set == 0 || index >= set->bit_count || set->words == 0)
    {
        return false;
    }
    const u32 word = bitset_word_index(index);
    const u32 mask = bitset_mask(index);
    if (value)
    {
        set->words[word] |= mask;
    }
    else
    {
        set->words[word] &= ~mask;
    }
    return true;
}

inline bool bitset_test(const Bitset *set, u32 index)
{
    if (set == 0 || index >= set->bit_count || set->words == 0)
    {
        return false;
    }
    const u32 word = bitset_word_index(index);
    const u32 mask = bitset_mask(index);
    return (set->words[word] & mask) != 0u;
}
