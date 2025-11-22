#include "mods/depgraph.h"

bool depgraph_resolve(const ModRegistry &registry, ModRegistry &resolved)
{
    resolved = registry;
    return true;
}
