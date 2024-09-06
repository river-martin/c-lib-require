#ifndef _REQUIRE_H
#define _REQUIRE_H

#include "../c-lib-trycatch/src/trycatch.h"
#include "requirement_error.h"
#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __REQUIRE_VOID_CAST (void)

/**
 * @brief Require that the given expression evaluates to true. If the expression
 * evaluates to false, throw a `RequirementError`.
 */
#define require(expr)                \
    ((expr) ? __REQUIRE_VOID_CAST(0) \
            : __require_fail(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__))

/**
 * @brief This function, when passed a string containing an asserted/required
 * expression, a filename, a line number, and function name, throws a
 * `RequirementError` with of the form:
 *
 * ```a.c:10: foobar: Requirement 'a == b' not met.```
 */
extern void __require_fail(const char  *__requirement,
                           const char  *__file,
                           unsigned int __line,
                           const char  *__function);

#endif // _REQUIRE_H
