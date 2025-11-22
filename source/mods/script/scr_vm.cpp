#include "mods/script/scr_vm.h"

bool scr_vm_init(ScriptVM *vm, const ScriptConfig &config)
{
    (void)config;
    if (vm == 0)
    {
        return false;
    }
    vm->dummy = 1u;
    return true;
}

void scr_vm_shutdown(ScriptVM *vm)
{
    if (vm)
    {
        vm->dummy = 0u;
    }
}

bool scr_vm_run_mod(ScriptVM *vm, const char *entry_path)
{
    (void)vm;
    (void)entry_path;
    /* Stub: always succeed. */
    return true;
}
