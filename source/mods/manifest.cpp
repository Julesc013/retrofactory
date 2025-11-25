#include "mods/manifest.h"

#include <cstring>
#include <cstdio>

#include "utility/path.h"
#include "utility/string.h"

namespace
{
    bool extract_json_string_range(const char *start, const char *end, const char *key, char *out, size_t out_size)
    {
        const char *found = std::strstr(start, key);
        if (found == 0)
        {
            return false;
        }
        found = std::strchr(found, ':');
        if (found == 0)
        {
            return false;
        }
        found += 1;
        while (*found == ' ' || *found == '\t' || *found == '\"')
        {
            ++found;
        }
        const char *cursor = found;
        while (cursor < end && *cursor != '\"' && *cursor != ',' && *cursor != '\n' && *cursor != '\r' && *cursor != ']')
        {
            ++cursor;
        }
        const size_t len = static_cast<size_t>(cursor - found);
        const size_t copy_len = len < out_size - 1 ? len : out_size - 1;
        std::memset(out, 0, out_size);
        std::memcpy(out, found, copy_len);
        out[copy_len] = '\0';
        return copy_len > 0;
    }

    bool extract_json_string(const char *buffer, const char *key, char *out, size_t out_size)
    {
        return extract_json_string_range(buffer, buffer + std::strlen(buffer), key, out, out_size);
    }

    u32 extract_json_u32(const char *buffer, const char *key, u32 fallback)
    {
        const char *found = std::strstr(buffer, key);
        if (found == 0)
        {
            return fallback;
        }
        found = std::strchr(found, ':');
        if (found == 0)
        {
            return fallback;
        }
        found += 1;
        while (*found == ' ' || *found == '\t' || *found == '\"')
        {
            ++found;
        }
        u32 value = fallback;
        std::sscanf(found, "%u", &value);
        return value;
    }

    u32 parse_string_list(const char *buffer, const char *key, char out[][kModNameMax], u32 max_out)
    {
        const char *found = std::strstr(buffer, key);
        if (found == 0)
        {
            return 0u;
        }
        const char *list_start = std::strchr(found, '[');
        const char *list_end = std::strchr(found, ']');
        if (list_start == 0 || list_end == 0 || list_end <= list_start)
        {
            return 0u;
        }

        u32 count = 0u;
        const char *cursor = list_start + 1;
        while (cursor < list_end && count < max_out)
        {
            while (cursor < list_end && (*cursor == ' ' || *cursor == '\t' || *cursor == ',' || *cursor == '\n' || *cursor == '\r'))
            {
                ++cursor;
            }
            if (cursor >= list_end)
            {
                break;
            }
            if (*cursor != '\"')
            {
                /* Skip invalid token. */
                ++cursor;
                continue;
            }
            ++cursor;
            const char *token_end = cursor;
            while (token_end < list_end && *token_end != '\"')
            {
                ++token_end;
            }
            const size_t len = static_cast<size_t>(token_end - cursor);
            const size_t copy_len = len < (kModNameMax - 1) ? len : (kModNameMax - 1);
            std::memcpy(out[count], cursor, copy_len);
            out[count][copy_len] = '\0';
            count += 1u;
            cursor = token_end + 1;
        }
        return count;
    }
}

bool mod_manifest_load(const char *path, ModManifest &out_manifest)
{
    if (path == 0 || *path == '\0')
    {
        return false;
    }

    std::memset(&out_manifest, 0, sizeof(ModManifest));

    StringBuffer manifest_path;
    string_buffer_init(&manifest_path, 256u);
    path_join(&manifest_path, path, "manifest.json");

    const char *manifest_cstr = string_buffer_c_str(&manifest_path);
    std::FILE *file = std::fopen(manifest_cstr, "rb");
    if (file == 0)
    {
        string_buffer_free(&manifest_path);
        return false;
    }

    std::fseek(file, 0, SEEK_END);
    const long size = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);
    if (size <= 0 || size > 4096)
    {
        std::fclose(file);
        string_buffer_free(&manifest_path);
        return false;
    }

    char *buffer = new char[static_cast<size_t>(size) + 1u];
    const size_t read = std::fread(buffer, 1u, static_cast<size_t>(size), file);
    buffer[read] = '\0';
    std::fclose(file);

    extract_json_string(buffer, "\"name\"", out_manifest.name, sizeof(out_manifest.name));
    if (!extract_json_string(buffer, "\"version\"", out_manifest.version, sizeof(out_manifest.version)))
    {
        std::strncpy(out_manifest.version, "0.0.0", kModVersionMax - 1);
    }
    out_manifest.priority = extract_json_u32(buffer, "\"priority\"", 100u);

    /* Derive fallback name from path. */
    if (out_manifest.name[0] == '\0')
    {
        StringView base;
        path_basename(path, base);
        const u32 copy_len = base.length < (kModNameMax - 1) ? base.length : (kModNameMax - 1);
        std::memcpy(out_manifest.name, base.data, copy_len);
        out_manifest.name[copy_len] = '\0';
    }

    std::strncpy(out_manifest.path, path, kModPathMax - 1);
    out_manifest.is_pack = std::strstr(path, "packs") != 0;

    out_manifest.dependency_count = parse_string_list(buffer, "\"dependencies\"", out_manifest.dependencies, kModMaxDeps);
    out_manifest.conflict_count = parse_string_list(buffer, "\"conflicts\"", out_manifest.conflicts, kModMaxDeps);

    delete[] buffer;
    string_buffer_free(&manifest_path);
    return mod_manifest_validate(out_manifest);
}

bool mod_manifest_validate(const ModManifest &manifest)
{
    if (manifest.name[0] == '\0')
    {
        return false;
    }
    u32 i;
    for (i = 0u; i < manifest.dependency_count; ++i)
    {
        if (std::strncmp(manifest.dependencies[i], manifest.name, kModNameMax) == 0)
        {
            return false;
        }
    }
    for (i = 0u; i < manifest.conflict_count; ++i)
    {
        if (std::strncmp(manifest.conflicts[i], manifest.name, kModNameMax) == 0)
        {
            return false;
        }
    }
    return true;
}
