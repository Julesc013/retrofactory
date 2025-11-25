#include "saveload/migrate.h"

bool migrate_save(u32 from_version, u32 to_version)
{
    /* Versions identical today; migrations will be added as formats evolve. */
    return from_version == to_version;
}
