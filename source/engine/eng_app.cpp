#include "engine/eng_app.h"

#include "core/core_api.h"

int engine_main_loop()
{
    CoreConfig config = {0u};
    CoreState state = {0u};

    if (!core_init(state, config))
    {
        return 1;
    }

    if (!core_tick(state))
    {
        core_shutdown(state);
        return 1;
    }

    core_shutdown(state);
    return 0;
}
