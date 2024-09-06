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

LDFLAGS := $(ASAN_FLAGS) -ltrycatch -lstack

# commands

COMPILE:=$(CC) $(CFLAGS) $(DFLAGS)

# targets

lib: build/librequire.a

tests: build $(ALL_OBJ_FILES) test_require

SRC_C_FILES := $(wildcard src/*.c)
SRC_OBJ_FILES := $(patsubst src/%.c, build/src/%.o, $(SRC_C_FILES))

TESTS_C_FILES := $(wildcard tests/*.c)
TESTS_H_FILES := $(wildcard tests/*.h)
TESTS_OBJ_FILES := $(patsubst tests/%.c, build/tests/%.o, $(TESTS_C_FILES))

ALL_C_FILES := $(SRC_C_FILES) $(TESTS_C_FILES)
ALL_H_FILES := $(SRC_H_FILES) $(TESTS_H_FILES)
ALL_OBJ_FILES := $(SRC_OBJ_FILES) $(TESTS_OBJ_FILES)

ALL_CODE_FILES := $(ALL_C_FILES) $(ALL_H_FILES)
# target files

build/librequire.a: $(SRC_OBJ_FILES) | build
	ar rcs build/librequire.a $(SRC_OBJ_FILES)

build/src/%.o: src/%.c $(SRC_C_FILES) $(SRC_H_FILES) | build
	$(COMPILE) -c -o $@ $<

build/tests/%.o: tests/%.c $(TESTS_C_FILES) $(TESTS_H_FILES) | build
	$(COMPILE) -c -o $@ $<

test_require:
	$(COMPILE) -o $@ tests/test_require.c $(LDFLAGS) -lrequire -ltrycatch

# target directories

build:
	mkdir -p build
	mkdir -p build/src
	mkdir -p build/tests

# phony targets

install: build/librequire.a src/require.h src/requirement_error.h
	install -d /usr/local/lib /usr/local/include
	# Octal permissions: 6 = rw-, 4 = r--
	install -m 644 build/librequire.a /usr/local/lib
	install -m 644 src/require.h /usr/local/include
	install -m 644 src/requirement_error.h /usr/local/include

uninstall:
	rm -f /usr/local/lib/librequire.a /usr/local/include/require.h


clean:
	rm -f test_require
	rm -f build/src/*.o build/tests/*.o
	rmdir build/src build/tests
	rm -f build/*.a
	rmdir build

.PHONY: install uninstall clean tests lib