#include "requirement_error.h"

RequirementError new_RequirementError(const char *msg_fmt, ...)
{
    va_list args;
    va_start(args, msg_fmt);
    RequirementError self = va_new_Error(msg_fmt, args);
    va_end(args);

    va_start(args, msg_fmt);
    __va_init_RequirementError(&self, msg_fmt, args);
    va_end(args);
    return self;
}

void __va_init_RequirementError(RequirementError *self,
                                const char       *msg_fmt,
                                va_list           args)
{
    assert(self->__init__);
    // Call the parent's __init__ method
    self->__init__(self, msg_fmt, args);
    self->type_name = "RequirementError";
    self->__init__  = &__va_init_RequirementError;
}
