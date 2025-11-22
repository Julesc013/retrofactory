#include "mods/loader.h"

#include <cstdio>
#include <cstring>

#include "mods/manifest.h"
#include "mods/depgraph.h"
#include "utility/path.h"
#include "utility/string.h"
#include "utility/hash.h"

namespace
{
    bool file_exists(const char *path)
    {
        std::FILE *f = std::fopen(path, "rb");
        if (f)
        {
            std::fclose(f);
            return true;
        }
        return false;
    }

    u32 read_mod_list(const char *list_path, char entries[][kModNameMax], u32 max_entries)
    {
        std::FILE *file = std::fopen(list_path, "rb");
        if (file == 0)
        {
            return 0u;
        }

        u32 count = 0u;
        char line[128];
        while (std::fgets(line, sizeof(line), file) != 0 && count < max_entries)
        {
            char *start = line;
            while (*start == ' ' || *start == '\t')
            {
                ++start;
            }
            if (*start == '#' || *start == '\0' || *start == '\n' || *start == '\r')
            {
                continue;
            }

            char *end = start;
            while (*end != '\0' && *end != '\n' && *end != '\r')
            {
                ++end;
            }
            *end = '\0';
            std::strncpy(entries[count], start, kModNameMax - 1);
            entries[count][kModNameMax - 1] = '\0';
            count += 1u;
        }
        std::fclose(file);
        return count;
    }

    u32 count_token_occurrences(const char *path, const char *token)
    {
        std::FILE *file = std::fopen(path, "rb");
        if (file == 0)
        {
            return 0u;
        }
        std::fseek(file, 0, SEEK_END);
        const long size = std::ftell(file);
        std::fseek(file, 0, SEEK_SET);
        if (size <= 0 || size > 32 * 1024)
        {
            std::fclose(file);
            return 0u;
        }

        char *buffer = new char[static_cast<size_t>(size) + 1u];
        const size_t read = std::fread(buffer, 1u, static_cast<size_t>(size), file);
        buffer[read] = '\0';
        std::fclose(file);

        u32 count = 0u;
        const char *cursor = buffer;
        const size_t token_len = std::strlen(token);
        while (cursor && *cursor != '\0')
        {
            const char *found = std::strstr(cursor, token);
            if (found == 0)
            {
                break;
            }
            count += 1u;
            cursor = found + token_len;
        }

        delete[] buffer;
        return count;
    }

    bool ingest_prototypes(ModRecord &record)
    {
        StringBuffer path_buf;
        string_buffer_init(&path_buf, 256u);

        const char *root = record.manifest.path;
        ModPrototypeSummary &summary = record.summary;

        path_join(&path_buf, root, "prototypes/entities.json");
        summary.entities = count_token_occurrences(string_buffer_c_str(&path_buf), "\"id\"");

        path_join(&path_buf, root, "prototypes/networks.json");
        summary.networks = count_token_occurrences(string_buffer_c_str(&path_buf), "\"id\"");

        path_join(&path_buf, root, "prototypes/recipes.json");
        summary.recipes = count_token_occurrences(string_buffer_c_str(&path_buf), "\"id\"");

        path_join(&path_buf, root, "prototypes/research.json");
        summary.research = count_token_occurrences(string_buffer_c_str(&path_buf), "\"id\"");

        path_join(&path_buf, root, "prototypes/worldgen.json");
        summary.worldgen = count_token_occurrences(string_buffer_c_str(&path_buf), "\"biome\"");

        string_buffer_free(&path_buf);
        return true;
    }
}

bool mod_loader_scan(ModRegistry &registry, const RuntimeConfig &config)
{
    mod_registry_init(registry);

    /* Attempt to use mods_default.cfg if present. */
    char entries[32][kModNameMax];
    u32 entry_count = 0u;

    StringBuffer list_path;
    string_buffer_init(&list_path, 256u);
    path_join(&list_path, "config", "mods_default.cfg");
    const char *list_cstr = string_buffer_c_str(&list_path);
    if (file_exists(list_cstr))
    {
        entry_count = read_mod_list(list_cstr, entries, 32u);
    }
    string_buffer_free(&list_path);

    /* Fallback to known built-in mods if none are configured. */
    static const char *kFallbackMods[] = {"base", "mercury", "venus", "earth", "mars", "jupiter", "saturn", "space"};
    if (entry_count == 0u)
    {
        u32 i;
        for (i = 0u; i < sizeof(kFallbackMods) / sizeof(kFallbackMods[0]); ++i)
        {
            std::strncpy(entries[entry_count], kFallbackMods[i], kModNameMax - 1);
            entries[entry_count][kModNameMax - 1] = '\0';
            entry_count += 1u;
        }
    }

    /* Also attempt a couple of default packs. */
    std::strncpy(entries[entry_count], "packs/gfx_classic", kModNameMax - 1);
    entries[entry_count][kModNameMax - 1] = '\0';
    entry_count += 1u;
    std::strncpy(entries[entry_count], "packs/sfx_classic", kModNameMax - 1);
    entries[entry_count][kModNameMax - 1] = '\0';
    entry_count += 1u;
    std::strncpy(entries[entry_count], "packs/mus_classic", kModNameMax - 1);
    entries[entry_count][kModNameMax - 1] = '\0';
    entry_count += 1u;

    StringBuffer mod_path;
    string_buffer_init(&mod_path, 256u);

    u32 idx;
    for (idx = 0u; idx < entry_count; ++idx)
    {
        string_buffer_clear(&mod_path);
        path_join(&mod_path, config.data_path, entries[idx]);

        ModManifest manifest;
        if (mod_manifest_load(string_buffer_c_str(&mod_path), manifest) && mod_manifest_validate(manifest))
        {
            if (!mod_registry_add(registry, manifest))
            {
                string_buffer_free(&mod_path);
                return false;
            }
        }
    }

    string_buffer_free(&mod_path);
    return registry.mod_count > 0u;
}

bool mod_loader_resolve(ModRegistry &registry)
{
    ModRegistry resolved;
    if (!depgraph_resolve(registry, resolved))
    {
        return false;
    }
    registry = resolved;
    return true;
}

bool mod_loader_apply(ModRegistry &registry)
{
    u32 i;
    for (i = 0u; i < registry.mod_count; ++i)
    {
        ModRecord &record = registry.mods[i];
        ingest_prototypes(record);
    }
    mod_registry_recalculate(registry);
    return true;
}
