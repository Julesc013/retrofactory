/* C-compatible so core/engine can see it */

#ifndef REND_BACKEND_H
#define REND_BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RenderBackendFamily {
    REND_FAMILY_SOFTWARE = 0,
    REND_FAMILY_SDL,      /* SDL1/SDL2 (2D API, GPU hidden) */
    REND_FAMILY_DX,       /* DirectX 2D/3D */
    REND_FAMILY_GL,       /* OpenGL 1/2/3 */
    REND_FAMILY_VK,       /* Vulkan 1/2 */
    REND_FAMILY_METAL,    /* Metal */
    REND_FAMILY_GDI,      /* Win16/Win32 GDI */
    REND_FAMILY_QDRAW,    /* MacOS Classic QuickDraw */
    REND_FAMILY_WEB,      /* Web canvas/WebGL/WebGPU shim */
    REND_FAMILY_COUNT
} RenderBackendFamily;

typedef enum RenderBackendVersion {
    REND_VER_NONE   = 0,

    /* SDL family */
    REND_VER_SDL1   = 10,
    REND_VER_SDL2   = 11,

    /* DirectX family */
    REND_VER_DX3    = 20,
    REND_VER_DX5    = 21,
    REND_VER_DX7    = 22,
    REND_VER_DX9    = 23,
    REND_VER_DX11   = 24,

    /* OpenGL */
    REND_VER_GL1    = 30,
    REND_VER_GL2    = 31,
    REND_VER_GL3    = 32,

    /* Vulkan */
    REND_VER_VK1    = 40,
    REND_VER_VK2    = 41,

    /* Metal */
    REND_VER_METAL1 = 50,
    REND_VER_METAL2 = 51
} RenderBackendVersion;

typedef struct RenderBackendId {
    RenderBackendFamily  family;
    RenderBackendVersion version;
} RenderBackendId;

typedef struct RenderBackendCaps {
    int  max_width;
    int  max_height;
    int  max_texture_size;
    int  supports_linear_filter;
    int  supports_aniso;
    int  supports_shader_effects;
    int  prefers_power_of_two;
    int  vsync_available;
    int  float_render_targets;
} RenderBackendCaps;

#ifdef __cplusplus
}
#endif

#endif /* REND_BACKEND_H */
