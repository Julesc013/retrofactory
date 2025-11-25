#include "launcher/common/launcher_model.h"

void launcher_model_init(LauncherModel &model, LauncherEdition edition)
{
    model.edition = edition;
    model.renderer.family = REND_FAMILY_SDL;
    model.renderer.version = REND_VER_SDL2;
}
