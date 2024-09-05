#include "custom_error.h"

const ErrType CUSTOM_ET /* extends BASE_ERR_TYPE */ = {
    .name      = "Custom",
    .supertype = &BASE_ERR_TYPE,
    .va_new_err =
        (void *(*) (const char *msg_fmt, va_list args)) va_new_CustomError,
    .new_err     = (void *(*) (const char *msg_fmt, ...)) new_CustomError,
    .init_err    = init_Error,
    .va_init_err = va_init_Error,
    .free_err    = free_Error,
};

void *new_CustomError(const char *msg_fmt, ...)
{
    va_list args;
    va_start(args, msg_fmt);
    Error *self = va_new_CustomError(msg_fmt, args);
    va_end(args);
    return self;
}

void *va_new_CustomError(const char *msg_fmt, va_list args)
{
    Error *self = va_new_Error(msg_fmt, args);
    self->type  = &CUSTOM_ET;
    return self;
}
