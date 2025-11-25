#pragma once

#include "system/types.h"

#include <new>
#include <cstring>

/* Minimal dynamic array without STL or exceptions. */
template <typename T>
struct Array
{
    T *data;
    u32 size;
    u32 capacity;

    Array() : data(0), size(0u), capacity(0u) {}
};

template <typename T>
inline void array_init(Array<T> &arr)
{
    arr.data = 0;
    arr.size = 0u;
    arr.capacity = 0u;
}

template <typename T>
inline void array_free(Array<T> &arr)
{
    if (arr.data != 0)
    {
        delete[] arr.data;
        arr.data = 0;
    }
    arr.size = 0u;
    arr.capacity = 0u;
}

template <typename T>
inline bool array_reserve(Array<T> &arr, u32 new_capacity)
{
    if (new_capacity <= arr.capacity)
    {
        return true;
    }

    T *next = new (std::nothrow) T[new_capacity];
    if (next == 0)
    {
        return false;
    }

    if (arr.data != 0 && arr.size > 0u)
    {
        u32 i;
        for (i = 0u; i < arr.size; ++i)
        {
            next[i] = arr.data[i];
        }
    }

    delete[] arr.data;
    arr.data = next;
    arr.capacity = new_capacity;
    return true;
}

template <typename T>
inline bool array_resize(Array<T> &arr, u32 new_size)
{
    if (new_size > arr.capacity)
    {
        const u32 grow = arr.capacity == 0u ? 4u : arr.capacity * 2u;
        u32 target = grow;
        if (target < new_size)
        {
            target = new_size;
        }
        if (!array_reserve(arr, target))
        {
            return false;
        }
    }

    if (new_size > arr.size)
    {
        /* Zero-init the tail to avoid uninitialised data in tests. */
        const u32 delta = new_size - arr.size;
        std::memset(arr.data + arr.size, 0, static_cast<size_t>(delta * sizeof(T)));
    }

    arr.size = new_size;
    return true;
}

template <typename T>
inline bool array_push(Array<T> &arr, const T &value)
{
    if (arr.size >= arr.capacity)
    {
        const u32 next_cap = arr.capacity == 0u ? 4u : arr.capacity * 2u;
        if (!array_reserve(arr, next_cap))
        {
            return false;
        }
    }

    arr.data[arr.size] = value;
    arr.size += 1u;
    return true;
}

template <typename T>
inline bool array_pop(Array<T> &arr, T *out_value)
{
    if (arr.size == 0u)
    {
        return false;
    }

    arr.size -= 1u;
    if (out_value != 0)
    {
        *out_value = arr.data[arr.size];
    }
    return true;
}

template <typename T>
inline void array_clear(Array<T> &arr)
{
    arr.size = 0u;
}

template <typename T>
inline T *array_begin(Array<T> &arr)
{
    return arr.data;
}

template <typename T>
inline T *array_end(Array<T> &arr)
{
    return arr.data != 0 ? arr.data + arr.size : 0;
}

template <typename T>
inline const T *array_begin(const Array<T> &arr)
{
    return arr.data;
}

template <typename T>
inline const T *array_end(const Array<T> &arr)
{
    return arr.data != 0 ? arr.data + arr.size : 0;
}
