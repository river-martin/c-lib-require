CC := clang

# Note: run `export LD_PRELOAD=$(clang -print-file-name=libclang_rt.asan-x86_64.so)` if you want to use Address Sanitizer
ASAN_FLAGS ?= #-fsanitize=address -shared-libasan

# compiler flags
DEBUG     := -ggdb -gdwarf-4 -ferror-limit=1 $(ASAN_FLAGS)
OPTIMISE  := -O0
WARNING   := -Wall -Wextra -Wno-variadic-macros \
             -Wno-overlength-strings -pedantic
EXTRA     := -std=c11 -fPIC
CFLAGS    := $(DEBUG) $(OPTIMISE) $(WARNING) $(EXTRA)
DFLAGS    ?= # -DDEBUG

# linker flags

LDFLAGS := $(ASAN_FLAGS)

# commands

COMPILE:=$(CC) $(CFLAGS) $(DFLAGS)

# files

LIBREQUIRE.A:= build/librequire.a
REQUIRE.O:=build/require.o

REQUIRE.C:=src/require.c
REQUIRE.H:=src/require.h

TEST_REQUIRE.C:=tests/test_require.c
TEST_TRY_CATCH.C:=tests/test_try_catch.c

# targets

# target files

$(LIBREQUIRE.A): $(REQUIRE.O) build
	ar rcs $(LIBREQUIRE.A) $(REQUIRE.O) $(LDFLAGS)

$(REQUIRE.O): $(REQUIRE.C) build
	$(COMPILE) -c -o $(REQUIRE.O) $(REQUIRE.C) $(LDFLAGS)

test_require: $(TEST_REQUIRE.C) build
	$(COMPILE) -o test_require $(TEST_REQUIRE.C) $(LDFLAGS) -lrequire

test_try_catch: $(REQUIRE.C) $(TEST_TRY_CATCH.C) build
	$(COMPILE) -o test_try_catch $(TEST_TRY_CATCH.C) $(LDFLAGS) -lrequire

# target directories

build:
	mkdir -p build

# phony targets

install: $(LIBREQUIRE.A) $(REQUIRE.H)
	install -d /usr/local/lib /usr/local/include
	# Octal permissions: 6 = rw-, 4 = r--
	install -m 644 $(LIBREQUIRE.A) /usr/local/lib
	install -m 644 $(REQUIRE.H) /usr/local/include

uninstall:
	rm -f /usr/local/lib/librequire.a /usr/local/include/require.h

clean:
	rm -f test_require
	rm -f test_try_catch
	rm -f build/*
	rmdir build

.PHONY: install uninstall clean