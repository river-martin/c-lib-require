#include <trycatcherror.h>
#include <assert.h>
#include "custom_error.h"
#include <execinfo.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    try {
        // Test nested try-catch blocks
        try {
            throw(CUSTOM_ET.new_err("Thrown from line %d\n", __LINE__ + 1));
        } catch (CUSTOM_ET) {
            printf("Caught error on line %d:\n\t%s", __LINE__, error->msg);
            CUSTOM_ET.free_err(error);
            error = CUSTOM_ET.new_err("Thrown from line %d\n", __LINE__ + 1);
            throw(error);
        }
        assert(("This should never be reached", false));
    } catch (CUSTOM_ET) {
        printf("Caught error on line %d:\n\t%s", __LINE__, error->msg);
    } catch (CUSTOM_ET) {
        assert(("This should never be reached", false));
    } catch (CUSTOM_ET) {
        assert(("This should never be reached", false));
    }
    return EXIT_SUCCESS;
}
