#include "require.h"

extern const char *__progname;

struct requirement_error requirement_err;
jmp_buf          jmp_env;
int              __jmp_env_set_count = 0;

/**
 * This function, when passed a string containing an asserted/required
 * expression, a filename, a line number, a jmp_buf for the error handling
 * environment, and a pointer to a RequirementError struct, sets the message in
 * the struct to something of the form: "a.c:10: foobar: Requirement `a == b'
 * not met.". It then does a longjmp to the most recent setjmp(env) (or
 * TRY(env), if you aren't calling setjmp(env) yourself), or it prints the
 * message to stderr and calls abort(). The function is called by the
 * __require_fail
 */
void __require_fail_base(const char       *__requirement,
                         const char       *__file,
                         unsigned int      __line,
                         const char       *__function,
                         jmp_buf          *__env,
                         struct requirement_error *error)
{
    int count = snprintf(error->msg, MAX_ERR_MSG_LEN,
                         "%s%s%s:%u: %s%sRequirement `%s` not met.\n",
                         __progname, __progname[0] ? ": " : "", __file, __line,
                         __function ? __function : "", __function ? ": " : "",
                         __requirement);
    assert(count < MAX_ERR_MSG_LEN);
    if (__jmp_env_set_count > 0) {
        // Jump back to the most recent setjmp(env)
        assert(__env);
        longjmp(*__env, 1);
    } else {
        fprintf(stderr, "%sNo jump set. Aborting...\n", error->msg);
        abort();
    }
}

void __require_fail(const char  *__requirement,
                    const char  *__file,
                    unsigned int __line,
                    const char  *__function)
{
    __require_fail_base(__requirement, __file, __line, __function, &jmp_env,
                        &requirement_err);
}
