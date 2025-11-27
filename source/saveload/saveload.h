#pragma once

#include "core/core.h"

struct SaveWriter;
struct SaveReader;
struct GameState;

bool save_core_state(const CoreState &state, const char *path);
bool load_core_state(CoreState &state, const char *path);

bool saveload_write_travel_networks(struct SaveWriter* w, const struct GameState* g);
bool saveload_read_travel_networks(struct SaveReader* r, struct GameState* g);

bool saveload_write_trans_networks(struct SaveWriter* w, const struct GameState* g);
bool saveload_read_trans_networks(struct SaveReader* r, struct GameState* g);
