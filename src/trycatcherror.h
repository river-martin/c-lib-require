#ifndef _TRYCATCHERROR_H
#define _TRYCATCHERROR_H

#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Macros

#define MAX_ERR_MSG_LEN 256
#define BT_BUF_SIZE     256

// Function macros

/**
 * Start a `try-catch` expression. The `try` block must be
 * followed by one or more `catch(err_type)` blocks.
 *
 * Usage: `try { ... } catch(err_type) { ... }`
 */
#define try                         \
    jmp_buf *__prev = __try_env;    \
    jmp_buf  __current_jmp_buf;     \
    __try_env = &__current_jmp_buf; \
    if (setjmp(*__try_env) == 0)

/**
 * Catch the error of type `err_type`.
 *
 * Usage: `try { ... } catch(err_type) { ... }`
 *
 * @param err_type The `const ErrType` of the error to catch
 */
#define catch(err_type) \
    else if ((__try_env = __prev, error && (&err_type == (error->type))))

/**
 * Usage: `throw(BASE_ERR_TYPE.new_err("Error message"))`
 *
 * @param err The `Error *` to throw
 */
#define throw(err)              \
    {                           \
        assert(!error || err == error);         \
        error = err;            \
        assert(__try_env);      \
        longjmp(*__try_env, 1); \
    }

// Type definitions

typedef struct et ErrType;

/**
 * The base error struct (associated with `const ErrType BASE_ERR_TYPE`). All
 * other error types should extend or use this struct.
 */
typedef struct {
    const ErrType *type;
    char          *msg;
    void          *backtrace_buf[BT_BUF_SIZE];
    int            backtrace_size;
} Error;

/**
 * @brief Defines the functions and metadata associated with
 * errors of this type.
 *
 * There should be one `const ErrType` for a given type of
 * error. For example, there is only one `const ErrType BASE_ERR_TYPE`, and it
 * is pointed to by all instances of `Error`.
 */
typedef struct et {
    const char *name; /**< The name of this type of error */
    const ErrType
        *supertype; /**< The supertype of this type of error (or `NULL`) */

    /**
     * @brief Allocates memory and initializes a new error of this type.
     * @param msg_fmt The format string for the error message
     * @param ...     The arguments to be formatted into the message string
     * @return A pointer to the newly allocated and initialized `Error`
     */
    void *(*new_err)(const char *msg_fmt, ...);

    /**
     * @brief Same as `new_err`, but takes a `va_list` instead of a variable
     * number of arguments.
     * @param msg_fmt The format string for the error message
     * @param args    The `va_list` of arguments to be formatted into the
     * message string
     * @return A pointer to the newly allocated and initialized `Error`.
     */
    void *(*va_new_err)(const char *msg_fmt, va_list args);

    /**
     * @brief Initializes an existing error of this type.
     * @param self    A pointer to the `Error` to initialize
     * @param msg_fmt The format string for the error message
     * @param ...     The arguments to be formatted into the message string
     */
    void (*init_err)(Error *self, const char *msg_fmt, ...);

    /**
     * @brief Same as `init_err`, but takes a `va_list` instead of a variable
     * number of arguments.
     * @param self    A pointer to the `Error` to initialize
     * @param msg_fmt The format string for the error message
     * @param args    The `va_list` of arguments to be formatted into the
     * message string
     */
    void (*va_init_err)(Error *self, const char *msg_fmt, va_list args);

    /**
     * @brief Frees the memory allocated for an error of this type.
     *
     * Does not free the memory
     * @param self A pointer to the `Error` to free.
     */
    void (*free_err)(Error *self);
} ErrType;

// Function prototypes

/**
 * Allocates memory for a new Error struct and initializes it's message field
 * with the given format string and arguments.
 *
 * Example usage:
 * `BASE_ERR_TYPE.new_err("%s:%d: %s", __FILE__, __LINE__, "An error
 * occurred");`
 *
 * @param msg_fmt The format string for the error message (see printf for
 * details)
 * @param ...     The arguments to be formatted into the message string
 */
extern Error *new_Error(const char *msg_fmt, ...);

/**
 * The same as new_Error, but takes a va_list instead of a variable number of
 * arguments.
 *
 * @param msg_fmt The format string for the error message (see printf for
 * details)
 * @param args    The va_list of arguments to be formatted into the message
 * string
 */
extern Error *va_new_Error(const char *msg_fmt, va_list args);

/**
 * Initializes the given Error * with the given format string and
 * arguments.
 *
 * Example usage:
 *  `error->type->init_err(error, "%s:%d: %s", __FILE__, __LINE__, "An error
 * occurred");` Which is equivalent to: `init_Error(error, "%s:%d: %s",
 * __FILE__, __LINE__, "An error occurred");` if error->type == &BASE_ERR_TYPE.
 *
 * @param self    The Error * to initialize
 * @param msg_fmt The format string for the error message (see printf for
 * details)
 * @param ...     The arguments to be formatted into the message string
 */
extern void init_Error(Error *self, const char *msg_fmt, ...);

/**
 * The same as init_Error, but takes a va_list instead of a variable number of
 * arguments.
 *
 * @param self    The Error * to initialize
 * @param msg_fmt The format string for the error message (see printf for
 */
extern void va_init_Error(Error *self, const char *msg_fmt, va_list args);

/**
 * Frees the memory allocated for the given Error *. This function should be
 * called when an error is no longer needed (i.e. at the end of a catch block).
 */
extern void free_Error(Error *self);

// Global variable declarations
extern const ErrType BASE_ERR_TYPE;
// Global error variable (set when an error is thrown)
extern Error        *error;

// Used in error messages
extern const char *__progname;

extern jmp_buf *__try_env;

#endif // _TRYCATCHERROR_H
