#include <cstdio>

bool win32_fs_exists(const char *path)
{
    std::FILE *f = std::fopen(path, "rb");
    if (f == 0)
    {
        return false;
    }
    std::fclose(f);
    return true;
}
