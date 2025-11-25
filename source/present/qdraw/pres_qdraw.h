#ifndef PRES_QDRAW_H
#define PRES_QDRAW_H

#include "system/types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool pres_qdraw_init(void);
bool pres_qdraw_present(void);
void pres_qdraw_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* PRES_QDRAW_H */
