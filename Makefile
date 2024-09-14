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

LDFLAGS := $(ASAN_FLAGS) -L. -lrequire

# commands

COMPILE:=$(CC) $(CFLAGS) $(DFLAGS)

# files

CODE_FILES := $(wildcard src/*.c src/*.h tests/*.c tests/*.h)
SRC_OBJ_FILES := $(patsubst src/%.c, build/src/%.o, $(wildcard src/*.c))
# targets

librequire.a: $(SRC_OBJ_FILES) dep-objs
	ar rcs librequire.a $(SRC_OBJ_FILES) $(wildcard build/dep-objs/*.o)
	ar t librequire.a

dep-objs: | build
	$(MAKE) -C c-lib-trycatch libtrycatch.a
	ar x c-lib-trycatch/libtrycatch.a --output build/dep-objs

build/src/%.o: src/%.c $(CODE_FILES) | build
	$(COMPILE) -c -o $@ $<

build/tests/%.o: tests/%.c $(CODE_FILES) | build
	$(COMPILE) -c -o $@ $<

test_require: librequire.a | build
	$(COMPILE) -o $@ tests/test_require.c $(LDFLAGS)

tests: test_require

# target directories

build:
	mkdir -p build/dep-objs
	mkdir -p build/src
	mkdir -p build/tests

# phony targets

install: librequire.a src/require.h src/requirement_error.h
	install -d /usr/local/lib /usr/local/include
	# Octal permissions: 6 = rw-, 4 = r--
	install -m 644 librequire.a /usr/local/lib
	install -m 644 src/require.h /usr/local/include
	install -m 644 src/requirement_error.h /usr/local/include

uninstall:
	rm -f /usr/local/lib/librequire.a /usr/local/include/require.h

clean:
	rm -f test_require
	rm -f librequire.a
	rm -rf build
	$(MAKE) -C c-lib-trycatch clean

.PHONY: install uninstall clean tests lib dep-objs