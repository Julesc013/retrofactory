#ifndef SW_CORE_H
#define SW_CORE_H

#include "system/types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool sw_core_init(void);
bool sw_core_present(void);
void sw_core_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* SW_CORE_H */
