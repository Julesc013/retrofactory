#include "launcher/common/launcher_detect.h"

LauncherEdition launcher_detect_default()
{
    /* Default to Nx (modern desktop) for stub builds. */
    return Edition_Nx;
}
