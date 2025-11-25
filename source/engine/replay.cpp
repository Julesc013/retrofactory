#include "engine/replay.h"

#include "utility/hash.h"
#include "world/world.h"

bool replay_record_frame(const SnapshotWorld &snapshot, ReplayFrame &out_frame)
{
    if (snapshot.world == 0)
    {
        return false;
    }
    out_frame.tick = snapshot.tick;

    u64 hash = 1469598103934665603ULL;
    const WorldDimensions &dim = snapshot.world->dimensions;
    hash = hash_bytes64(&dim, sizeof(WorldDimensions));
    if (snapshot.world->tiles != 0 && snapshot.world->tile_count > 0u)
    {
        const size_t bytes = static_cast<size_t>(snapshot.world->tile_count * sizeof(Tile));
        const u64 tiles_hash = hash_bytes64(snapshot.world->tiles, bytes);
        hash ^= tiles_hash + (hash << 6) + (hash >> 2);
    }
    hash ^= snapshot.tick + (hash << 1);

    out_frame.hash = hash;
    return true;
}
