#pragma once

#include "system/types.h"

/* Lightweight immutable view. */
struct StringView
{
    const char *data;
    u32 length;

    StringView() : data(0), length(0u) {}
    StringView(const char *ptr, u32 len) : data(ptr), length(len) {}
};

/* Owning buffer with fixed capacity and manual growth. */
struct StringBuffer
{
    char *data;
    u32 length;
    u32 capacity;

    StringBuffer() : data(0), length(0u), capacity(0u) {}
};

StringView string_view_from_cstr(const char *cstr);
bool string_view_equals(const StringView &a, const StringView &b, bool case_sensitive = true);
bool string_view_starts_with(const StringView &view, const StringView &prefix);

bool string_buffer_init(StringBuffer *buf, u32 capacity);
void string_buffer_free(StringBuffer *buf);
void string_buffer_clear(StringBuffer *buf);
bool string_buffer_assign(StringBuffer *buf, const char *text);
bool string_buffer_append(StringBuffer *buf, const char *text);
bool string_buffer_append_n(StringBuffer *buf, const char *text, u32 len);
bool string_buffer_append_char(StringBuffer *buf, char c);
bool string_buffer_append_u32(StringBuffer *buf, u32 value);
const char *string_buffer_c_str(StringBuffer *buf);
