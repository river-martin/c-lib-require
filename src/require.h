#ifndef _REQUIRE_H
#define _REQUIRE_H

#include "trycatcherror.h"
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
 * evaluates to false, throw a `RequirementError` (if in a `try` block) or print
 * the error message to `stderr` and call `abort` (if not in a a `try` block).
 */
#define require(expr)                \
    ((expr) ? __REQUIRE_VOID_CAST(0) \
            : __require_fail(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__))

/**
 * @brief The error type used when a requirement is not met.
 */
extern const ErrType REQ_ERR_TYPE;

/**
 * @brief This function, when passed a string containing an asserted/required
 * expression, a filename, a line number, and function name, creates a
 * `RequirementError` with of the form:
 *
 * ```a.c:10: foobar: Requirement 'a == b' not met.```
 *
 * If called from inside a `try` block, it then throws the error. If called from
 * outside a `try` block, it prints the error message to `stderr` and calls
 * `abort`.
 */
extern void __require_fail(const char  *__requirement,
                           const char  *__file,
                           unsigned int __line,
                           const char  *__function);

#endif // _REQUIRE_H
