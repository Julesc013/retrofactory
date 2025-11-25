#ifndef RETRO_RUNTIME_H
#define RETRO_RUNTIME_H

#include "system/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RetroRuntime RetroRuntime;

typedef enum RetroPresenter
{
    RETRO_PRESENTER_SOFTWARE = 0,
    RETRO_PRESENTER_GDI = 1,
    RETRO_PRESENTER_QDRAW = 2
} RetroPresenter;

/* Create/destroy a retro runtime instance. Width/height seed the camera. */
RetroRuntime *retro_runtime_create(u32 camera_width, u32 camera_height, RetroPresenter presenter);
void retro_runtime_destroy(RetroRuntime *rt);

/* Initialise engine and buffers; config_path may be null to use defaults. */
bool retro_runtime_init(RetroRuntime *rt, const char *config_path);

/* Advance one tick and render a frame into the internal backbuffer. */
bool retro_runtime_step(RetroRuntime *rt);

/* Update camera dimensions for subsequent frames. */
void retro_runtime_set_camera(RetroRuntime *rt, u32 camera_width, u32 camera_height);

#ifdef __cplusplus
}
#endif

#endif /* RETRO_RUNTIME_H */
