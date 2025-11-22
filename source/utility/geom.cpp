#include "utility/geom.h"

Recti recti_make(i32 x, i32 y, i32 w, i32 h)
{
    Recti r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

Recti recti_from_size(Size2i size)
{
    return recti_make(0, 0, size.w, size.h);
}

Recti recti_intersect(const Recti &a, const Recti &b)
{
    const i32 ax2 = a.x + a.w;
    const i32 ay2 = a.y + a.h;
    const i32 bx2 = b.x + b.w;
    const i32 by2 = b.y + b.h;

    const i32 x1 = a.x > b.x ? a.x : b.x;
    const i32 y1 = a.y > b.y ? a.y : b.y;
    const i32 x2 = ax2 < bx2 ? ax2 : bx2;
    const i32 y2 = ay2 < by2 ? ay2 : by2;

    Recti result;
    result.x = x1;
    result.y = y1;
    result.w = x2 - x1;
    result.h = y2 - y1;
    if (result.w < 0)
    {
        result.w = 0;
    }
    if (result.h < 0)
    {
        result.h = 0;
    }
    return result;
}

bool recti_contains_point(const Recti &r, Vec2i p)
{
    return p.x >= r.x && p.x < (r.x + r.w) && p.y >= r.y && p.y < (r.y + r.h);
}

bool recti_is_empty(const Recti &r)
{
    return r.w <= 0 || r.h <= 0;
}

Size2i recti_size(const Recti &r)
{
    Size2i s;
    s.w = r.w;
    s.h = r.h;
    return s;
}

Rectf rectf_make(float x, float y, float w, float h)
{
    Rectf r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

Rectf rectf_from_recti(const Recti &r)
{
    return rectf_make(static_cast<float>(r.x), static_cast<float>(r.y),
                      static_cast<float>(r.w), static_cast<float>(r.h));
}

bool rectf_contains_point(const Rectf &r, Vec2f p)
{
    return p.x >= r.x && p.x < (r.x + r.w) && p.y >= r.y && p.y < (r.y + r.h);
}
