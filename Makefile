# Makefile wrapper for CMake-based build/test workflow

CMAKE ?= cmake
CTEST ?= ctest
BUILD_DIR ?= build
USE_SYSTEM_GTEST ?= ON
ENABLE_COVERAGE ?= OFF
LIZARD ?= python3 -m lizard
CLOC ?= cloc

.PHONY: all configure build test clean rebuild coverage complexity loc quality

all: test

configure:
	$(CMAKE) -S . -B $(BUILD_DIR) \
		-DUSE_SYSTEM_GTEST=$(USE_SYSTEM_GTEST) \
		-DCHILD_LOCK_ENABLE_COVERAGE=$(ENABLE_COVERAGE)

build: configure
	$(CMAKE) --build $(BUILD_DIR)

test: build
	$(CTEST) --test-dir $(BUILD_DIR) --output-on-failure

coverage: clean
	$(MAKE) test ENABLE_COVERAGE=ON

complexity:
	$(LIZARD) -C 10 src include tests

loc:
	$(CLOC) src include tests

quality: complexity loc

rebuild: clean test

clean:
	$(CMAKE) -E rm -rf $(BUILD_DIR)
