#include "mods/manifest.h"

#include <cstring>
#include <cstdio>

#include "utility/path.h"
#include "utility/string.h"

namespace
{
    bool extract_json_string(const char *buffer, const char *key, char *out, size_t out_size)
    {
        const char *found = std::strstr(buffer, key);
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
        const char *end = found;
        while (*end != '\0' && *end != '\"' && *end != ',' && *end != '\n' && *end != '\r')
        {
            ++end;
        }
        const size_t len = static_cast<size_t>(end - found);
        const size_t copy_len = len < out_size - 1 ? len : out_size - 1;
        std::memset(out, 0, out_size);
        std::memcpy(out, found, copy_len);
        out[copy_len] = '\0';
        return copy_len > 0;
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
    out_manifest.priority = 0u;
    out_manifest.is_pack = std::strstr(path, "packs") != 0;

    delete[] buffer;
    string_buffer_free(&manifest_path);
    return mod_manifest_validate(out_manifest);
}

bool mod_manifest_validate(const ModManifest &manifest)
{
    return manifest.name[0] != 0;
}
