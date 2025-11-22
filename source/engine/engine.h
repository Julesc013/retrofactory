#pragma once

#include "core/core.h"
#include "engine/config.h"
#include "engine/state.h"

struct EngineContext
{
    EngineConfig config;
    EngineState state;
    CoreConfig core_config;
    CoreState core_state;
};

bool engine_init(EngineContext &ctx, const char *config_path);
void engine_shutdown(EngineContext &ctx);
bool engine_tick(EngineContext &ctx);
