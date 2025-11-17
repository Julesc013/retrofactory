#include "engine/eng_app.h"

#include "interface/launch_main.h"
#include "runtime/rt_log.h"

int engine_main_loop()
{
    log_info("Engine dispatching to launch subsystem");
    const int result = launch_run();
    log_info("Engine main loop completed");
    return result;
}
