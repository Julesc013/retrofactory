#include "mods/loader.h"

#include <cstdio>
#include <cstring>

#include "mods/manifest.h"
#include "mods/depgraph.h"
#include "mods/prototypes.h"
#include "utility/path.h"
#include "utility/string.h"
#include "utility/hash.h"
#include "runtime/rt_log.h"

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

    char *load_file_text(const char *path, size_t max_size, size_t &out_size)
    {
        out_size = 0u;
        std::FILE *file = std::fopen(path, "rb");
        if (file == 0)
        {
            return 0;
        }

        std::fseek(file, 0, SEEK_END);
        const long size = std::ftell(file);
        std::fseek(file, 0, SEEK_SET);
        if (size <= 0 || static_cast<size_t>(size) > max_size)
        {
            std::fclose(file);
            return 0;
        }

        char *buffer = new char[static_cast<size_t>(size) + 1u];
        out_size = std::fread(buffer, 1u, static_cast<size_t>(size), file);
        buffer[out_size] = '\0';
        std::fclose(file);
        return buffer;
    }

    bool parse_string_range(const char *start, const char *end, const char *key, char *out, size_t out_cap)
    {
        const char *found = std::strstr(start, key);
        if (found == 0 || found >= end)
        {
            return false;
        }
        found = std::strchr(found, ':');
        if (found == 0 || found >= end)
        {
            return false;
        }
        found += 1;
        while (found < end && (*found == ' ' || *found == '\t' || *found == '\"'))
        {
            ++found;
        }
        const char *cursor = found;
        while (cursor < end && *cursor != '\"' && *cursor != ',' && *cursor != '\n' && *cursor != '\r' && *cursor != ']')
        {
            ++cursor;
        }
        const size_t len = static_cast<size_t>(cursor - found);
        const size_t copy_len = len < (out_cap - 1) ? len : (out_cap - 1);
        std::memset(out, 0, out_cap);
        if (copy_len > 0u)
        {
            std::memcpy(out, found, copy_len);
            out[copy_len] = '\0';
        }
        return copy_len > 0u;
    }

    u32 parse_u32_range(const char *start, const char *end, const char *key, u32 fallback)
    {
        const char *found = std::strstr(start, key);
        if (found == 0 || found >= end)
        {
            return fallback;
        }
        found = std::strchr(found, ':');
        if (found == 0 || found >= end)
        {
            return fallback;
        }
        found += 1;
        while (found < end && (*found == ' ' || *found == '\t' || *found == '\"'))
        {
            ++found;
        }
        u32 value = fallback;
        std::sscanf(found, "%u", &value);
        return value;
    }

    bool parse_entities_json(const char *buffer, const ModManifest &manifest, PrototypeStore &store, ModPrototypeSummary &summary)
    {
        const char *cursor = buffer;
        while ((cursor = std::strstr(cursor, "\"id\"")) != 0)
        {
            const char *obj_start = cursor;
            const char *obj_end = std::strchr(obj_start, '}');
            if (obj_end == 0)
            {
                break;
            }

            EntityPrototype proto;
            std::memset(&proto, 0, sizeof(EntityPrototype));
            if (!parse_string_range(obj_start, obj_end, "\"id\"", proto.id, kProtoIdMax))
            {
                cursor = obj_end + 1;
                continue;
            }

            parse_string_range(obj_start, obj_end, "\"sprite\"", proto.sprite, kProtoIdMax);
            parse_string_range(obj_start, obj_end, "\"pack\"", proto.pack, kProtoPackMax);
            if (proto.pack[0] == '\0' && manifest.is_pack)
            {
                std::strncpy(proto.pack, manifest.name, kProtoPackMax - 1);
            }
            proto.speed = parse_u32_range(obj_start, obj_end, "\"speed\"", 1u);
            proto.sprite_index = hash_string32(proto.sprite[0] ? proto.sprite : proto.id);

            if (prototype_upsert_entity(store, proto))
            {
                summary.entities += 1u;
            }
            cursor = obj_end + 1;
        }
        return true;
    }

    bool parse_networks_json(const char *buffer, PrototypeStore &store, ModPrototypeSummary &summary)
    {
        const char *cursor = buffer;
        while ((cursor = std::strstr(cursor, "\"id\"")) != 0)
        {
            const char *obj_start = cursor;
            const char *obj_end = std::strchr(obj_start, '}');
            if (obj_end == 0)
            {
                break;
            }

            NetworkPrototype proto;
            std::memset(&proto, 0, sizeof(NetworkPrototype));
            if (!parse_string_range(obj_start, obj_end, "\"id\"", proto.id, kProtoIdMax))
            {
                cursor = obj_end + 1;
                continue;
            }
            proto.capacity = parse_u32_range(obj_start, obj_end, "\"capacity\"", 100u);
            if (prototype_upsert_network(store, proto))
            {
                summary.networks += 1u;
            }
            cursor = obj_end + 1;
        }
        return true;
    }

    bool parse_recipes_json(const char *buffer, PrototypeStore &store, ModPrototypeSummary &summary)
    {
        const char *cursor = buffer;
        while ((cursor = std::strstr(cursor, "\"id\"")) != 0)
        {
            const char *obj_start = cursor;
            const char *obj_end = std::strchr(obj_start, '}');
            if (obj_end == 0)
            {
                break;
            }

            RecipePrototype proto;
            std::memset(&proto, 0, sizeof(RecipePrototype));
            if (!parse_string_range(obj_start, obj_end, "\"id\"", proto.id, kProtoIdMax))
            {
                cursor = obj_end + 1;
                continue;
            }
            proto.time = parse_u32_range(obj_start, obj_end, "\"time\"", 1u);
            proto.input_cost = parse_u32_range(obj_start, obj_end, "\"input\"", 1u);
            proto.output_yield = parse_u32_range(obj_start, obj_end, "\"output\"", 1u);
            if (prototype_upsert_recipe(store, proto))
            {
                summary.recipes += 1u;
            }
            cursor = obj_end + 1;
        }
        return true;
    }

    bool parse_research_json(const char *buffer, PrototypeStore &store, ModPrototypeSummary &summary)
    {
        const char *cursor = buffer;
        while ((cursor = std::strstr(cursor, "\"id\"")) != 0)
        {
            const char *obj_start = cursor;
            const char *obj_end = std::strchr(obj_start, '}');
            if (obj_end == 0)
            {
                break;
            }

            ResearchPrototype proto;
            std::memset(&proto, 0, sizeof(ResearchPrototype));
            if (!parse_string_range(obj_start, obj_end, "\"id\"", proto.id, kProtoIdMax))
            {
                cursor = obj_end + 1;
                continue;
            }
            proto.cost = parse_u32_range(obj_start, obj_end, "\"cost\"", 50u);
            if (prototype_upsert_research(store, proto))
            {
                summary.research += 1u;
            }
            cursor = obj_end + 1;
        }
        return true;
    }

    bool parse_worldgen_json(const char *buffer, PrototypeStore &store, ModPrototypeSummary &summary)
    {
        const char *cursor = buffer;
        while ((cursor = std::strstr(cursor, "\"biome\"")) != 0)
        {
            const char *obj_start = cursor;
            const char *obj_end = std::strchr(obj_start, '}');
            if (obj_end == 0)
            {
                break;
            }

            WorldgenPrototype proto;
            std::memset(&proto, 0, sizeof(WorldgenPrototype));
            if (!parse_string_range(obj_start, obj_end, "\"biome\"", proto.biome, kProtoIdMax))
            {
                cursor = obj_end + 1;
                continue;
            }
            proto.weight = parse_u32_range(obj_start, obj_end, "\"weight\"", 1u);
            proto.terrain = static_cast<u8>(parse_u32_range(obj_start, obj_end, "\"terrain\"", summary.worldgen % 4u));
            if (prototype_upsert_worldgen(store, proto))
            {
                summary.worldgen += 1u;
            }
            cursor = obj_end + 1;
        }
        return true;
    }

    void ingest_prototypes(ModRecord &record, PrototypeStore &store)
    {
        StringBuffer path_buf;
        string_buffer_init(&path_buf, 256u);

        const char *root = record.manifest.path;
        ModPrototypeSummary &summary = record.summary;
        std::memset(&summary, 0, sizeof(ModPrototypeSummary));

        size_t size = 0u;

        path_join(&path_buf, root, "prototypes/entities.json");
        char *data = load_file_text(string_buffer_c_str(&path_buf), 64u * 1024u, size);
        if (data != 0)
        {
            parse_entities_json(data, record.manifest, store, summary);
            delete[] data;
        }

        string_buffer_clear(&path_buf);
        path_join(&path_buf, root, "prototypes/networks.json");
        data = load_file_text(string_buffer_c_str(&path_buf), 64u * 1024u, size);
        if (data != 0)
        {
            parse_networks_json(data, store, summary);
            delete[] data;
        }

        string_buffer_clear(&path_buf);
        path_join(&path_buf, root, "prototypes/recipes.json");
        data = load_file_text(string_buffer_c_str(&path_buf), 64u * 1024u, size);
        if (data != 0)
        {
            parse_recipes_json(data, store, summary);
            delete[] data;
        }

        string_buffer_clear(&path_buf);
        path_join(&path_buf, root, "prototypes/research.json");
        data = load_file_text(string_buffer_c_str(&path_buf), 64u * 1024u, size);
        if (data != 0)
        {
            parse_research_json(data, store, summary);
            delete[] data;
        }

        string_buffer_clear(&path_buf);
        path_join(&path_buf, root, "prototypes/worldgen.json");
        data = load_file_text(string_buffer_c_str(&path_buf), 64u * 1024u, size);
        if (data != 0)
        {
            parse_worldgen_json(data, store, summary);
            delete[] data;
        }

        string_buffer_free(&path_buf);
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

    /* Default packs to always include basic art/sound/music. */
    std::strncpy(entries[entry_count], "packs/gfx_pcx8_vga", kModNameMax - 1);
    entries[entry_count][kModNameMax - 1] = '\0';
    entry_count += 1u;
    std::strncpy(entries[entry_count], "packs/sfx_pcm4_4m", kModNameMax - 1);
    entries[entry_count][kModNameMax - 1] = '\0';
    entry_count += 1u;
    std::strncpy(entries[entry_count], "packs/mus_ogg22_m", kModNameMax - 1);
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

bool mod_loader_apply(ModRegistry &registry, PrototypeStore &prototypes)
{
    prototype_reset(prototypes);
    u32 i;
    for (i = 0u; i < registry.mod_count; ++i)
    {
        ModRecord &record = registry.mods[i];
        ingest_prototypes(record, prototypes);
    }

    mod_registry_recalculate(registry);
    registry.totals.entities = prototypes.entities.size;
    registry.totals.networks = prototypes.networks.size;
    registry.totals.recipes = prototypes.recipes.size;
    registry.totals.research = prototypes.research.size;
    registry.totals.worldgen = prototypes.worldgen.size;
    return true;
}
