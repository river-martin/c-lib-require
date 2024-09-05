#include "trycatcherror.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

jmp_buf *__try_env = NULL;
Error   *error     = NULL;

const ErrType BASE_ERR_TYPE = {
    .name        = "Error",
    .supertype   = NULL,
    .new_err     = (void *(*) (const char *msg_fmt, ...)) new_Error,
    .va_new_err  = (void *(*) (const char *msg_fmt, va_list args)) va_new_Error,
    .init_err    = init_Error,
    .va_init_err = va_init_Error,
    .free_err    = free_Error,
};

Error *new_Error(const char *msg_fmt, ...)
{
    va_list args;
    va_start(args, msg_fmt);
    Error *self = va_new_Error(msg_fmt, args);
    va_end(args);
    return self;
}

Error *va_new_Error(const char *msg_fmt, va_list args)
{
    Error *self = malloc(sizeof(Error));
    self->type  = &BASE_ERR_TYPE;
    self->msg   = malloc(MAX_ERR_MSG_LEN * sizeof(char));
    self->type->va_init_err(self, msg_fmt, args);
    return self;
}

void init_Error(Error *self, const char *msg_fmt, ...)
{
    va_list args;
    va_start(args, msg_fmt);
    va_init_Error(self, msg_fmt, args);
    va_end(args);
}

void va_init_Error(Error *self, const char *msg_fmt, va_list args)
{
    int count = vsnprintf(self->msg, MAX_ERR_MSG_LEN, msg_fmt, args);
    assert(count < MAX_ERR_MSG_LEN);
    self->backtrace_size = 0;
}

void free_Error(Error *self)
{
    assert(self);
    if (self->msg) {
        free(self->msg);
        self->msg = NULL;
    }
    free(self);
}
