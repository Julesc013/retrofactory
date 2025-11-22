#include "runtime/rt_config.h"

#include <cstring>
#include <cstdio>

namespace
{
    void apply_kv(RuntimeConfig *cfg, const char *key, const char *value)
    {
        if (std::strcmp(key, "data_path") == 0)
        {
            std::strncpy(cfg->data_path, value, kRfMaxPath - 1);
        }
        else if (std::strcmp(key, "mods_path") == 0)
        {
            std::strncpy(cfg->mods_path, value, kRfMaxPath - 1);
        }
        else if (std::strcmp(key, "save_path") == 0)
        {
            std::strncpy(cfg->save_path, value, kRfMaxPath - 1);
        }
        else if (std::strcmp(key, "logs_path") == 0)
        {
            std::strncpy(cfg->logs_path, value, kRfMaxPath - 1);
        }
        else if (std::strcmp(key, "edition") == 0)
        {
            std::strncpy(cfg->edition, value, kRfMaxName - 1);
        }
        else if (std::strcmp(key, "presenter") == 0)
        {
            std::strncpy(cfg->presenter, value, kRfMaxName - 1);
        }
        else if (std::strcmp(key, "render_backend") == 0)
        {
            std::strncpy(cfg->render_backend, value, kRfMaxName - 1);
        }
        else if (std::strcmp(key, "audio_backend") == 0)
        {
            std::strncpy(cfg->audio_backend, value, kRfMaxName - 1);
        }
    }
}

bool rt_config_load(RuntimeConfig *out_config, const char *path)
{
    if (out_config == 0)
    {
        return false;
    }

    std::memset(out_config, 0, sizeof(RuntimeConfig));

    if (path != 0)
    {
        std::strncpy(out_config->config_path, path, kRfMaxPath - 1);
        std::FILE *file = std::fopen(path, "rb");
        if (file != 0)
        {
            char line[256];
            while (std::fgets(line, sizeof(line), file) != 0)
            {
                /* Strip whitespace and comments. */
                char *start = line;
                while (*start == ' ' || *start == '\t')
                {
                    ++start;
                }
                if (*start == '#' || *start == '\0' || *start == '\n' || *start == '\r')
                {
                    continue;
                }
                char *sep = std::strchr(start, '=');
                if (sep == 0)
                {
                    continue;
                }
                *sep = '\0';
                char *value = sep + 1;
                char *end = value;
                while (*end != '\0' && *end != '\n' && *end != '\r')
                {
                    ++end;
                }
                *end = '\0';
                apply_kv(out_config, start, value);
            }
            std::fclose(file);
        }
    }
    if (out_config->data_path[0] == 0)
    {
        std::strncpy(out_config->data_path, "data", kRfMaxPath - 1);
    }
    if (out_config->save_path[0] == 0)
    {
        std::strncpy(out_config->save_path, "saves", kRfMaxPath - 1);
    }
    if (out_config->mods_path[0] == 0)
    {
        std::strncpy(out_config->mods_path, "data/mods", kRfMaxPath - 1);
    }
    if (out_config->logs_path[0] == 0)
    {
        std::strncpy(out_config->logs_path, "logs", kRfMaxPath - 1);
    }
    if (out_config->edition[0] == 0)
    {
        std::strncpy(out_config->edition, "base", kRfMaxName - 1);
    }
    if (out_config->presenter[0] == 0)
    {
        std::strncpy(out_config->presenter, "sdl2", kRfMaxName - 1);
    }
    if (out_config->render_backend[0] == 0)
    {
        std::strncpy(out_config->render_backend, "software", kRfMaxName - 1);
    }
    if (out_config->audio_backend[0] == 0)
    {
        std::strncpy(out_config->audio_backend, "silent", kRfMaxName - 1);
    }
    return true;
}

bool rt_config_merge(RuntimeConfig *base, const RuntimeConfig &override_config)
{
    if (base == 0)
    {
        return false;
    }

    if (override_config.config_path[0] != 0)
    {
        std::strncpy(base->config_path, override_config.config_path, kRfMaxPath - 1);
    }
    if (override_config.data_path[0] != 0)
    {
        std::strncpy(base->data_path, override_config.data_path, kRfMaxPath - 1);
    }
    if (override_config.save_path[0] != 0)
    {
        std::strncpy(base->save_path, override_config.save_path, kRfMaxPath - 1);
    }
    if (override_config.mods_path[0] != 0)
    {
        std::strncpy(base->mods_path, override_config.mods_path, kRfMaxPath - 1);
    }
    if (override_config.logs_path[0] != 0)
    {
        std::strncpy(base->logs_path, override_config.logs_path, kRfMaxPath - 1);
    }
    if (override_config.edition[0] != 0)
    {
        std::strncpy(base->edition, override_config.edition, kRfMaxName - 1);
    }
    if (override_config.presenter[0] != 0)
    {
        std::strncpy(base->presenter, override_config.presenter, kRfMaxName - 1);
    }
    if (override_config.render_backend[0] != 0)
    {
        std::strncpy(base->render_backend, override_config.render_backend, kRfMaxName - 1);
    }
    if (override_config.audio_backend[0] != 0)
    {
        std::strncpy(base->audio_backend, override_config.audio_backend, kRfMaxName - 1);
    }
    return true;
}
