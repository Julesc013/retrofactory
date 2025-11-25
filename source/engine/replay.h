#pragma once

#include "engine/snapshot.h"

struct ReplayFrame
{
    Tick tick;
    u64 hash;
};

bool replay_record_frame(const SnapshotWorld &snapshot, ReplayFrame &out_frame);
