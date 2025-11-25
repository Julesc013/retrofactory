#pragma once

#include "system/types.h"
#include "utility/vector.h"

struct Size2i
{
    i32 w;
    i32 h;
};

struct Recti
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
};

struct Rectf
{
    float x;
    float y;
    float w;
    float h;
};

Recti recti_make(i32 x, i32 y, i32 w, i32 h);
Recti recti_from_size(Size2i size);
Recti recti_intersect(const Recti &a, const Recti &b);
bool recti_contains_point(const Recti &r, Vec2i p);
bool recti_is_empty(const Recti &r);
Size2i recti_size(const Recti &r);

Rectf rectf_make(float x, float y, float w, float h);
Rectf rectf_from_recti(const Recti &r);
bool rectf_contains_point(const Rectf &r, Vec2f p);
