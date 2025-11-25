#include "launcher/ui/gdi_ui.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    return launcher_gdi_ui_run() ? 0 : 1;
}
