#pragma once

#include "system/types.h"

u32 hash_bytes32(const void *data, u32 size);
u64 hash_bytes64(const void *data, u32 size);
u32 hash_string32(const char *str);
u32 hash_combine32(u32 a, u32 b);
