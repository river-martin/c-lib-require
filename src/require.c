#include "require.h"

const ErrType REQ_ERR_TYPE = {
    .name        = "RequirementError",
    .supertype   = &BASE_ERR_TYPE,
    .va_new_err  = (void *(*) (const char *msg_fmt, va_list args)) va_new_Error,
    .new_err     = (void *(*) (const char *msg_fmt, ...)) new_Error,
    .init_err    = init_Error,
    .va_init_err = va_init_Error,
    .free_err    = free_Error,
};

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
    Error *err = REQ_ERR_TYPE.new_err("%s: %s:%u: %s: %s\n", __progname, __file,
                                      __line, __function, str);
    if (__try_env) {
        throw(err);
    } else {
        fprintf(stderr, "%sNot in a try block. Aborting...\n", err->msg);
        REQ_ERR_TYPE.free_err(err);
        abort();
    }
}
