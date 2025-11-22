#include "system/types.h"

namespace
{
    u32 g_frames = 0u;
}

bool posix_audio_init()
{
    g_frames = 0u;
    return true;
}

void posix_audio_mix_silence(u32 frames)
{
    g_frames += frames;
}

u32 posix_audio_frames()
{
    return g_frames;
}
