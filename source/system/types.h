#pragma once

#include <stdint.h>
#include <stddef.h>

/* Canonical fixed-width aliases used across the sim core. */
typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;
typedef size_t   usize;

/* Legacy-style aliases kept for convenience in tests and tools. */
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;

typedef u32 Tick;

/* Forward declarations to break header cycles in sim layers. */
struct World;
struct CoreState;
