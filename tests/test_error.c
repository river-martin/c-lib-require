#include <assert.h>
#include <execinfo.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <trycatcherror.h>

#include "custom_error.h"

void test_new_err(void)
{
    Error *error = CUSTOM_ET.new_err("Test error message");
    assert(error);
    assert(error->type == &CUSTOM_ET);
    assert(strcmp(error->msg, "Test error message") == 0);
    CUSTOM_ET.free_err(error);

    error = CUSTOM_ET.new_err("Test error message with format %s", "string");
    assert(error);
    assert(error->type == &CUSTOM_ET);
    assert(strcmp(error->msg, "Test error message with format string") == 0);
    CUSTOM_ET.free_err(error);
    printf("%s: passed\n", __func__);
}

int main(void)
{
    test_new_err();
    return EXIT_SUCCESS;
}
