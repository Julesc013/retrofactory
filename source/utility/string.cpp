#include "utility/string.h"

#include <new>
#include <cstring>
#include <cstdio>

StringView string_view_from_cstr(const char *cstr)
{
    if (cstr == 0)
    {
        return StringView();
    }
    const u32 len = static_cast<u32>(std::strlen(cstr));
    return StringView(cstr, len);
}

static bool chars_equal(char a, char b, bool case_sensitive)
{
    if (case_sensitive)
    {
        return a == b;
    }

    /* ASCII-only case fold for determinism. */
    if (a >= 'A' && a <= 'Z')
    {
        a = static_cast<char>(a - 'A' + 'a');
    }
    if (b >= 'A' && b <= 'Z')
    {
        b = static_cast<char>(b - 'A' + 'a');
    }
    return a == b;
}

bool string_view_equals(const StringView &a, const StringView &b, bool case_sensitive)
{
    if (a.length != b.length)
    {
        return false;
    }
    if (a.data == 0 || b.data == 0)
    {
        return a.data == b.data;
    }
    u32 i;
    for (i = 0u; i < a.length; ++i)
    {
        if (!chars_equal(a.data[i], b.data[i], case_sensitive))
        {
            return false;
        }
    }
    return true;
}

bool string_view_starts_with(const StringView &view, const StringView &prefix)
{
    if (view.length < prefix.length || view.data == 0 || prefix.data == 0)
    {
        return false;
    }
    u32 i;
    for (i = 0u; i < prefix.length; ++i)
    {
        if (view.data[i] != prefix.data[i])
        {
            return false;
        }
    }
    return true;
}

static bool ensure_capacity(StringBuffer *buf, u32 required)
{
    if (buf == 0)
    {
        return false;
    }

    if (required <= buf->capacity)
    {
        return true;
    }

    const u32 grow = buf->capacity == 0u ? 64u : buf->capacity * 2u;
    u32 target = grow;
    if (target < required)
    {
        target = required;
    }

    char *next = new (std::nothrow) char[target];
    if (next == 0)
    {
        return false;
    }

    if (buf->data != 0 && buf->length > 0u)
    {
        std::memcpy(next, buf->data, static_cast<size_t>(buf->length));
    }

    delete[] buf->data;
    buf->data = next;
    buf->capacity = target;
    return true;
}

bool string_buffer_init(StringBuffer *buf, u32 capacity)
{
    if (buf == 0)
    {
        return false;
    }
    buf->data = 0;
    buf->length = 0u;
    buf->capacity = 0u;
    if (capacity == 0u)
    {
        return true;
    }
    return ensure_capacity(buf, capacity);
}

void string_buffer_free(StringBuffer *buf)
{
    if (buf == 0)
    {
        return;
    }
    if (buf->data != 0)
    {
        delete[] buf->data;
        buf->data = 0;
    }
    buf->length = 0u;
    buf->capacity = 0u;
}

void string_buffer_clear(StringBuffer *buf)
{
    if (buf == 0)
    {
        return;
    }
    buf->length = 0u;
    if (buf->data != 0 && buf->capacity > 0u)
    {
        buf->data[0] = '\0';
    }
}

static void terminate(StringBuffer *buf)
{
    if (buf != 0 && buf->data != 0 && buf->capacity > 0u)
    {
        const u32 index = buf->length < buf->capacity ? buf->length : buf->capacity - 1u;
        buf->data[index] = '\0';
    }
}

bool string_buffer_assign(StringBuffer *buf, const char *text)
{
    if (buf == 0)
    {
        return false;
    }
    string_buffer_clear(buf);
    if (text == 0)
    {
        terminate(buf);
        return true;
    }
    const u32 len = static_cast<u32>(std::strlen(text));
    if (!ensure_capacity(buf, len + 1u))
    {
        return false;
    }
    std::memcpy(buf->data, text, static_cast<size_t>(len));
    buf->length = len;
    terminate(buf);
    return true;
}

bool string_buffer_append_n(StringBuffer *buf, const char *text, u32 len)
{
    if (buf == 0 || text == 0 || len == 0u)
    {
        return buf != 0;
    }
    const u32 needed = buf->length + len + 1u;
    if (!ensure_capacity(buf, needed))
    {
        return false;
    }
    std::memcpy(buf->data + buf->length, text, static_cast<size_t>(len));
    buf->length += len;
    terminate(buf);
    return true;
}

bool string_buffer_append(StringBuffer *buf, const char *text)
{
    if (text == 0)
    {
        return true;
    }
    return string_buffer_append_n(buf, text, static_cast<u32>(std::strlen(text)));
}

bool string_buffer_append_char(StringBuffer *buf, char c)
{
    if (buf == 0)
    {
        return false;
    }
    const u32 needed = buf->length + 2u;
    if (!ensure_capacity(buf, needed))
    {
        return false;
    }
    buf->data[buf->length] = c;
    buf->length += 1u;
    terminate(buf);
    return true;
}

bool string_buffer_append_u32(StringBuffer *buf, u32 value)
{
    char scratch[16];
    std::sprintf(scratch, "%u", value);
    return string_buffer_append(buf, scratch);
}

const char *string_buffer_c_str(StringBuffer *buf)
{
    if (buf == 0)
    {
        return "";
    }
    terminate(buf);
    return buf->data != 0 ? buf->data : "";
}
