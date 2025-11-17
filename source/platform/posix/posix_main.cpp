#include "engine/eng_app.h"
#include "platform/plat_api.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    plat_services services;
    services.log_message = 0;
    services.sleep_ms = 0;

    if (!plat_init(services))
    {
        return 1;
    }

    const int result = engine_main_loop();
    plat_shutdown();
    return result;
}
