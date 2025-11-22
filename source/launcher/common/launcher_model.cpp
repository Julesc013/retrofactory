#include "launcher/common/launcher_model.h"

void launcher_model_init(LauncherModel &model, LauncherEdition edition)
{
    model.edition = edition;
    model.renderer = 0u;
}
