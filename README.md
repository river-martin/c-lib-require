# C Lib Require

This library defines the macros `require(expression)`, `TRY`, and `CATCH(requirement_err)`. The `require` macro is similar to `assert` from `glibc`. However, if the expression evaluates to `false`, `longjmp` is called instead of `abort`. This allows C programmers to `CATCH` the `requirement_err` when a requirement is not met. If there is no destination to jump to (i.e `require` is used outside of a `TRY` block), `require(expression)` behaves in the same way as `assert(expression)`; it calles `abort`. See the files in `tests/` for code examples, and `src/require.h` for the definition of the `requirement_error` struct.

## Installation

```Bash
make
sudo make install
```

To uninstall the library, run: `sudo make uninstall`.

## Testing

```Bash
make test_require test_try_catch
./test_require
./test_try_catch
```

## Usage

Include the header file in your C code, and use the `require(expr)`, `TRY` and `CATCH(requirement_err)` macros. For example:

```C
#include <require.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {
  require(true);
  TRY {
    require(false);
  } CATCH(requirement_err) {
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
