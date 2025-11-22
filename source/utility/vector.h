#pragma once

#include "system/types.h"

struct Vec2i
{
    i32 x;
    i32 y;
};

struct Vec2u
{
    u32 x;
    u32 y;
};

struct Vec2f
{
    float x;
    float y;
};

inline Vec2i vec2i(i32 x, i32 y) { Vec2i v; v.x = x; v.y = y; return v; }
inline Vec2u vec2u(u32 x, u32 y) { Vec2u v; v.x = x; v.y = y; return v; }
inline Vec2f vec2f(float x, float y) { Vec2f v; v.x = x; v.y = y; return v; }

inline Vec2i vec2i_add(Vec2i a, Vec2i b) { return vec2i(a.x + b.x, a.y + b.y); }
inline Vec2i vec2i_sub(Vec2i a, Vec2i b) { return vec2i(a.x - b.x, a.y - b.y); }
inline Vec2u vec2u_add(Vec2u a, Vec2u b) { return vec2u(a.x + b.x, a.y + b.y); }
inline Vec2u vec2u_sub(Vec2u a, Vec2u b) { return vec2u(a.x - b.x, a.y - b.y); }
inline Vec2f vec2f_add(Vec2f a, Vec2f b) { return vec2f(a.x + b.x, a.y + b.y); }
inline Vec2f vec2f_sub(Vec2f a, Vec2f b) { return vec2f(a.x - b.x, a.y - b.y); }
inline Vec2f vec2f_scale(Vec2f a, float s) { return vec2f(a.x * s, a.y * s); }
