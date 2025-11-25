#pragma once

#include "system/types.h"

typedef struct SpriteDrawCmd {
    i32 x;
    i32 y;
    u32 sprite_id;
    u32 tint_rgba; /* optional tint colour */
} SpriteDrawCmd;

typedef struct QuadDrawCmd {
    float x0;
    float y0;
    float x1;
    float y1;
    u32 fill_rgba;
} QuadDrawCmd;
