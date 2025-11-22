#pragma once

#include "system/types.h"

template <typename T>
struct Span
{
    T *data;
    u32 length;

    Span() : data(0), length(0u) {}
    Span(T *ptr, u32 len) : data(ptr), length(len) {}

    T &operator[](u32 index) { return data[index]; }
    const T &operator[](u32 index) const { return data[index]; }
};

template <typename T>
inline Span<T> make_span(T *ptr, u32 len)
{
    return Span<T>(ptr, len);
}
