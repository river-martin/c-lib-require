#include <require.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void something_that_will_fail(void) { require(("this will fail", false)); }

void something_that_wont_fail(void) { require(("this will pass", 1 == 1)); }

int main(void)
{
    assert(("this will pass", 1 == 1));
    try {
        something_that_wont_fail();
        something_that_will_fail();
    } catch(requirement_err) {
        printf("%s", requirement_err.msg);
    }
    return EXIT_SUCCESS;
}
