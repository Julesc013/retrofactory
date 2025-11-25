#include "system/types.h"

namespace
{
    u32 g_sample_rate = 44100u;
    u32 g_submitted_frames = 0u;
}

bool win32_audio_init(u32 sample_rate)
{
    g_sample_rate = sample_rate;
    g_submitted_frames = 0u;
    return true;
}

void win32_audio_submit_silence(u32 frames)
{
    g_submitted_frames += frames;
}

u32 win32_audio_frames_submitted()
{
    return g_submitted_frames;
}
