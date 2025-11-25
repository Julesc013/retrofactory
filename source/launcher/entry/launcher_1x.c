#include "launcher/ui/text_ui.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    return launcher_text_ui_run() ? 0 : 1;
}
