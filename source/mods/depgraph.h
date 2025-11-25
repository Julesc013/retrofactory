#pragma once

#include "mods/registry.h"

/* Stub depgraph: currently returns registry order unchanged. */
bool depgraph_resolve(const ModRegistry &registry, ModRegistry &resolved);
