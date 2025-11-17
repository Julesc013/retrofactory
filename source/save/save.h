#ifndef SAVE_SAVE_H
#define SAVE_SAVE_H

#include "core/core_api.h"

bool save_core_state(const CoreState &state, const char *path);
bool load_core_state(CoreState &state, const char *path);

#endif /* SAVE_SAVE_H */
