#pragma once

/* C API surface exposed to script bindings. */
struct ModContext
{
    int placeholder;
};

int mod_api_spawn_entity(ModContext *, int proto_id, int x, int y);
