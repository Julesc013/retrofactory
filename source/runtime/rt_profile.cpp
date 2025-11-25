#include "runtime/rt_profile.h"

void rt_profile_begin(RtProfileScope &scope, const char *name)
{
    scope.name = name;
    scope.start_ticks = 0u;
}

void rt_profile_end(RtProfileScope &/*scope*/)
{
    /* Stub; no-op */
}
