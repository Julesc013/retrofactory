#include "utility/path.h"

#include <cstring>

static PathStyle detect_style(const char *path, PathStyle requested)
{
    if (requested != PathStyle_Auto)
    {
        return requested;
    }
    if (path != 0)
    {
        const char *iter = path;
        while (*iter != '\0')
        {
            if (*iter == '\\')
            {
                return PathStyle_Windows;
            }
            if (*iter == '/')
            {
                return PathStyle_Posix;
            }
            ++iter;
        }
    }
    return PathStyle_Posix;
}

char path_separator(PathStyle style)
{
    return detect_style(0, style) == PathStyle_Windows ? '\\' : '/';
}

bool path_is_separator(char c)
{
    return c == '/' || c == '\\';
}

bool path_is_absolute(const char *path)
{
    if (path == 0 || *path == '\0')
    {
        return false;
    }
    if (path[0] == '/' || path[0] == '\\')
    {
        return true;
    }
    if (((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')) &&
        path[1] == ':' &&
        path_is_separator(path[2]))
    {
        return true;
    }
    return false;
}

static void push_sep(StringBuffer *out, PathStyle style)
{
    if (out->length == 0u)
    {
        return;
    }
    const char sep = path_separator(style);
    const char last = out->data[out->length - 1u];
    if (!path_is_separator(last))
    {
        string_buffer_append_char(out, sep);
    }
}

static void collapse_separators(StringBuffer *io, PathStyle style)
{
    if (io->data == 0 || io->length == 0u)
    {
        return;
    }
    const char sep = path_separator(style);
    u32 write = 0u;
    u32 read;
    for (read = 0u; read < io->length; ++read)
    {
        char c = io->data[read];
        if (path_is_separator(c))
        {
            c = sep;
            if (write > 0u && io->data[write - 1u] == sep)
            {
                continue;
            }
        }
        io->data[write++] = c;
    }
    io->length = write;
    string_buffer_append_char(io, '\0');
    if (io->length > 0u)
    {
        io->length -= 1u;
    }
}

bool path_join(StringBuffer *out, const char *a, const char *b, PathStyle style)
{
    if (out == 0)
    {
        return false;
    }
    string_buffer_clear(out);
    if (a != 0 && *a != '\0')
    {
        string_buffer_append(out, a);
    }
    if (b != 0 && *b != '\0')
    {
        if (out->length > 0u && !path_is_separator(out->data[out->length - 1u]))
        {
            string_buffer_append_char(out, path_separator(style));
        }
        string_buffer_append(out, b);
    }
    path_normalize(out, style);
    return true;
}

bool path_join3(StringBuffer *out, const char *a, const char *b, const char *c, PathStyle style)
{
    if (!path_join(out, a, b, style))
    {
        return false;
    }
    return path_append_component(out, c, style);
}

bool path_append_component(StringBuffer *io, const char *component, PathStyle style)
{
    if (io == 0 || component == 0 || *component == '\0')
    {
        return false;
    }
    push_sep(io, style);
    string_buffer_append(io, component);
    path_normalize(io, style);
    return true;
}

bool path_replace_extension(StringBuffer *io, const char *new_ext)
{
    if (io == 0 || io->data == 0)
    {
        return false;
    }

    i32 last_dot = -1;
    i32 last_sep = -1;
    i32 i;
    for (i = static_cast<i32>(io->length) - 1; i >= 0; --i)
    {
        const char c = io->data[i];
        if (c == '.')
        {
            last_dot = i;
            break;
        }
        if (path_is_separator(c))
        {
            last_sep = i;
            break;
        }
    }

    if (last_dot == -1 || (last_sep != -1 && last_dot < last_sep))
    {
        last_dot = static_cast<i32>(io->length);
    }
    io->length = static_cast<u32>(last_dot);
    io->data[io->length] = '\0';

    if (new_ext != 0)
    {
        if (*new_ext != '.')
        {
            string_buffer_append_char(io, '.');
        }
        string_buffer_append(io, new_ext);
    }
    return true;
}

void path_normalize(StringBuffer *io, PathStyle style)
{
    if (io == 0 || io->data == 0)
    {
        return;
    }
    const PathStyle resolved = detect_style(io->data, style);
    collapse_separators(io, resolved);
}

bool path_dirname(const char *path, StringView &out)
{
    if (path == 0)
    {
        out = StringView();
        return false;
    }

    const u32 len = static_cast<u32>(std::strlen(path));
    if (len == 0u)
    {
        out = StringView();
        return false;
    }

    i32 i;
    for (i = static_cast<i32>(len) - 1; i >= 0; --i)
    {
        if (path_is_separator(path[i]))
        {
            const u32 dir_len = static_cast<u32>(i);
            out = StringView(path, dir_len);
            return true;
        }
    }

    out = StringView(path, 0u);
    return true;
}

bool path_basename(const char *path, StringView &out)
{
    if (path == 0)
    {
        out = StringView();
        return false;
    }
    const u32 len = static_cast<u32>(std::strlen(path));
    if (len == 0u)
    {
        out = StringView();
        return false;
    }
    i32 start = static_cast<i32>(len) - 1;
    for (; start >= 0; --start)
    {
        if (path_is_separator(path[start]))
        {
            start += 1;
            break;
        }
    }
    if (start < 0)
    {
        start = 0;
    }
    out = StringView(path + start, len - static_cast<u32>(start));
    return true;
}
