#include "../src/require.h"

void test_requirement_error(void)
{
    bool caught = false;
    try {
        require(true);
        assert(true);
        require(false);
    } catch (RequirementError, err) {
        caught = true;
        fprintf(stdout, "%s", err.msg);
    }
    assert(caught);
    printf("%s: passed\n", __func__);
}

void test_require(void)
{
    bool caught = false;
    try {
        require(true);
    } catch (RequirementError, err) {
        caught = true;
        assert(&err);
    }
    assert(!caught);
    try {
        require(false);
    } catch (RequirementError, err) {
        caught = true;
        assert(&err);
    }
    assert(caught);
    printf("%s: passed\n", __func__);
}

int main(void)
{
    test_requirement_error();
    test_require();
    return 0;
}
