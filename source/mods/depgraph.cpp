#include "mods/depgraph.h"

#include <cstring>

#include "runtime/rt_log.h"

namespace
{
    bool has_dependency(const ModManifest &manifest, const char *name)
    {
        u32 i;
        for (i = 0u; i < manifest.dependency_count; ++i)
        {
            if (std::strncmp(manifest.dependencies[i], name, kModNameMax) == 0)
            {
                return true;
            }
        }
        return false;
    }

    bool has_conflict(const ModManifest &manifest, const char *name)
    {
        u32 i;
        for (i = 0u; i < manifest.conflict_count; ++i)
        {
            if (std::strncmp(manifest.conflicts[i], name, kModNameMax) == 0)
            {
                return true;
            }
        }
        return false;
    }

    bool registry_contains(const ModRegistry &registry, const char *name)
    {
        u32 i;
        for (i = 0u; i < registry.mod_count; ++i)
        {
            if (std::strncmp(registry.mods[i].manifest.name, name, kModNameMax) == 0)
            {
                return true;
            }
        }
        return false;
    }

    bool deps_exist(const ModRecord &record, const ModRegistry &registry)
    {
        u32 i;
        for (i = 0u; i < record.manifest.dependency_count; ++i)
        {
            if (!registry_contains(registry, record.manifest.dependencies[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool deps_resolved(const ModRecord &record, const ModRegistry &resolved)
    {
        u32 i;
        for (i = 0u; i < record.manifest.dependency_count; ++i)
        {
            if (!registry_contains(resolved, record.manifest.dependencies[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool conflicts_present(const ModRecord &record, const ModRegistry &registry)
    {
        u32 i;
        for (i = 0u; i < record.manifest.conflict_count; ++i)
        {
            if (registry_contains(registry, record.manifest.conflicts[i]))
            {
                return true;
            }
        }
        return false;
    }

    int compare_candidates(const ModRecord &a, const ModRecord &b)
    {
        if (a.manifest.priority != b.manifest.priority)
        {
            return a.manifest.priority < b.manifest.priority ? -1 : 1;
        }
        return std::strcmp(a.manifest.name, b.manifest.name);
    }
}

bool depgraph_resolve(const ModRegistry &registry, ModRegistry &resolved)
{
    mod_registry_init(resolved);

    /* Validate dependencies and conflicts up-front. */
    u32 idx;
    for (idx = 0u; idx < registry.mod_count; ++idx)
    {
        const ModRecord &rec = registry.mods[idx];
        if (!deps_exist(rec, registry))
        {
            rt_log_error("Missing dependency in mod graph");
            return false;
        }
        if (conflicts_present(rec, registry))
        {
            rt_log_error("Conflict detected in mod graph");
            return false;
        }
    }

    bool taken[kMaxMods];
    std::memset(taken, 0, sizeof(taken));

    while (resolved.mod_count < registry.mod_count)
    {
        int best_index = -1;
        ModRecord best_record;

        for (idx = 0u; idx < registry.mod_count; ++idx)
        {
            if (taken[idx])
            {
                continue;
            }

            const ModRecord &candidate = registry.mods[idx];
            if (!deps_resolved(candidate, resolved))
            {
                continue;
            }

            if (best_index < 0 || compare_candidates(candidate, best_record) < 0)
            {
                best_index = static_cast<i32>(idx);
                best_record = candidate;
            }
        }

        if (best_index < 0)
        {
            /* Cyclic dependency or unsatisfied constraint. */
            return false;
        }

        resolved.mods[resolved.mod_count] = best_record;
        resolved.mod_count += 1u;
        taken[best_index] = true;
    }

    mod_registry_recalculate(resolved);
    return resolved.mod_count == registry.mod_count;
}
