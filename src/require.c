#include "require.h"

extern const char *__progname;

void __require_fail(const char  *__requirement,
                    const char  *__file,
                    unsigned int __line,
                    const char  *__function)
{
    char str[MAX_ERR_MSG_LEN];
    int  count = snprintf(str, MAX_ERR_MSG_LEN, "Requirement '%s' not met.",
                          __requirement);
    assert(count < MAX_ERR_MSG_LEN);
    throw(new_RequirementError("%s: %s:%u: %s: %s\n", __progname, __file,
                               __line, __function, str));
}
