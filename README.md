# c-lib-require

This library defines the macro `require(expression)` and the type `RequirementError`. The `require` macro is similar to `assert` from `glibc`. However, if the expression evaluates to `false`, a `RequirementError` is thrown (using the `throw` macro defined in `trycatch.h`) instead of calling `abort`.

## Installation

```Bash
make
sudo make install
```

To uninstall the library, run: `sudo make uninstall`.

## Testing

```Bash
make tests
./test_require
```

## Usage

Include the header file in your C code, and use the `require(expr)`, `try` and `catch(requirement_err)` macros. For example:

```C
#include <require.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {
  require(true);
  try {
    require(false);
  } catch(requirement_err) {
    printf("%s", requirement_err.msg);
  }
  return 0;
}
```

Link against the library when you compile your code.

```Bash
clang -o your_code your_code.c -lrequire
```

## Acknowledgements

This library is based on `glibc`'s implementation of `assert`. The copy and license information for `glibc` is provided in `GLIBC-COPYING` and `GLIBC-LICENSES`.
