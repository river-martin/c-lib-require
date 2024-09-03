#ifndef _REQUIRE_H
#define _REQUIRE_H

#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ERR_MSG_LEN 256

struct requirement_error {
    const char  *requirement;
    const char  *file;
    unsigned int line;
    const char  *function;
    char         msg[MAX_ERR_MSG_LEN];
};

extern struct requirement_error requirement_err;
extern jmp_buf          jmp_env;
extern int              __jmp_env_set_count;

extern void __require_fail(const char  *__requirement,
                           const char  *__file,
                           unsigned int __line,
                           const char  *__function);

#define __REQUIRE_VOID_CAST (void)

#define require(expr)                \
    ((expr) ? __REQUIRE_VOID_CAST(0) \
            : __require_fail(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__))

// Usage: try { ... } catch(requirement_err) { ... }
#define try                    if ((__jmp_env_set_count++, setjmp(jmp_env)) == 0)
// Usage: try { ... } catch(requirement_err) { ... }
#define catch(requirement_err) else if ((__jmp_env_set_count--, true))

#endif // _REQUIRE_H
