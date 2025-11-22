#include "engine/engine.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    EngineContext engine;
    if (!engine_init(engine, 0))
    {
        return 1;
    }

    engine_tick(engine);
    engine_shutdown(engine);
    return 0;
}
