#ifndef PRES_GDI_H
#define PRES_GDI_H

#include "system/types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool pres_gdi_init(void);
bool pres_gdi_present(void);
void pres_gdi_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* PRES_GDI_H */
