#ifndef REQUIREMENT_ERROR_H
#define REQUIREMENT_ERROR_H

#include "../c-lib-trycatch/src/trycatch.h"

/**
 * @brief The type of error used when a requirement is not met.
 */
typedef Error RequirementError;

extern RequirementError new_RequirementError(const char *msg_fmt, ...);

extern void __va_init_RequirementError(RequirementError *self,
                                       const char       *msg_fmt,
                                       va_list           args);

#endif // REQUIREMENT_ERROR_H
