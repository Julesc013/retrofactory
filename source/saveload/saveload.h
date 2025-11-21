#pragma once

#include "core/core.h"

bool save_core_state(const CoreState &state, const char *path);
bool load_core_state(CoreState &state, const char *path);
