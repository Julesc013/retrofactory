#pragma once

#include "utility/string.h"

enum PathStyle
{
    PathStyle_Auto,
    PathStyle_Posix,
    PathStyle_Windows
};

char path_separator(PathStyle style = PathStyle_Auto);
bool path_is_separator(char c);
bool path_is_absolute(const char *path);

bool path_join(StringBuffer *out, const char *a, const char *b, PathStyle style = PathStyle_Auto);
bool path_join3(StringBuffer *out, const char *a, const char *b, const char *c, PathStyle style = PathStyle_Auto);
bool path_append_component(StringBuffer *io, const char *component, PathStyle style = PathStyle_Auto);
bool path_replace_extension(StringBuffer *io, const char *new_ext);

void path_normalize(StringBuffer *io, PathStyle style = PathStyle_Auto);
bool path_dirname(const char *path, StringView &out);
bool path_basename(const char *path, StringView &out);
