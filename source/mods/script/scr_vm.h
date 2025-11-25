#pragma once

#include "system/types.h"

struct ScriptVM
{
    u32 dummy;
};

struct ScriptConfig
{
    u32 stack_size;
};

bool scr_vm_init(ScriptVM *vm, const ScriptConfig &config);
void scr_vm_shutdown(ScriptVM *vm);
bool scr_vm_run_mod(ScriptVM *vm, const char *entry_path);
