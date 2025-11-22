#include "system/rng.h"

int main()
{
    RngStream rng_a;
    RngStream rng_b;
    rng_seed(&rng_a, 1234u, 1u);
    rng_seed(&rng_b, 1234u, 1u);

    u32 i;
    for (i = 0u; i < 16u; ++i)
    {
        if (rng_next_u32(&rng_a) != rng_next_u32(&rng_b))
        {
            return 1;
        }
    }

    rng_seed(&rng_b, 1234u, 2u);
    if (rng_next_u32(&rng_a) == rng_next_u32(&rng_b))
    {
        return 1;
    }

    return 0;
}
