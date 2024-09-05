#ifndef CUSTOM_ERROR_H
#define CUSTOM_ERROR_H

#include <trycatcherror.h>

extern const ErrType CUSTOM_ET;

extern void *va_new_CustomError(const char *msg_fmt, va_list args);

extern void *new_CustomError(const char *msg_fmt, ...);

#endif // CUSTOM_ERROR_H
