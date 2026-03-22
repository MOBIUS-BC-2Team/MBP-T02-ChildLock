# Makefile wrapper for CMake-based build/test workflow

CMAKE ?= cmake
CTEST ?= ctest
BUILD_DIR ?= build
USE_SYSTEM_GTEST ?= ON
ENABLE_COVERAGE ?= OFF
GCOVR ?= gcovr
GCOV ?= gcov-11
COVERAGE_DIR ?= $(BUILD_DIR)/coverage
LIZARD ?= python3 -m lizard
CLOC ?= cloc
CPPCHECK ?= cppcheck

.PHONY: all configure build test clean rebuild coverage coverage-report complexity loc cppcheck quality

all: test

configure:
	$(CMAKE) -S . -B $(BUILD_DIR) \
		-DUSE_SYSTEM_GTEST=$(USE_SYSTEM_GTEST) \
		-DCHILD_LOCK_ENABLE_COVERAGE=$(ENABLE_COVERAGE)

build: configure
	$(CMAKE) --build $(BUILD_DIR)

test: build
	$(CTEST) --test-dir $(BUILD_DIR) --output-on-failure
ifeq ($(ENABLE_COVERAGE),ON)
	$(MAKE) coverage-report BUILD_DIR=$(BUILD_DIR) COVERAGE_DIR=$(COVERAGE_DIR) GCOVR=$(GCOVR)
endif

coverage: clean
	$(MAKE) test ENABLE_COVERAGE=ON

coverage-report:
	$(CMAKE) -E make_directory $(COVERAGE_DIR)
	@if command -v $(GCOVR) >/dev/null 2>&1; then \
		echo "[coverage] using gcovr detailed report"; \
		$(GCOVR) -r . $(BUILD_DIR) \
			--exclude '.*/tests/.*' \
			--exclude '.*/build/_deps/.*' \
			--print-summary; \
		$(GCOVR) -r . $(BUILD_DIR) \
			--exclude '.*/tests/.*' \
			--exclude '.*/build/_deps/.*' \
			--txt --output $(COVERAGE_DIR)/coverage.txt; \
		$(GCOVR) -r . $(BUILD_DIR) \
			--exclude '.*/tests/.*' \
			--exclude '.*/build/_deps/.*' \
			--html-details --output $(COVERAGE_DIR)/index.html; \
		$(GCOVR) -r . $(BUILD_DIR) \
			--exclude '.*/tests/.*' \
			--exclude '.*/build/_deps/.*' \
			--xml-pretty --output $(COVERAGE_DIR)/coverage.xml; \
		echo "[coverage] detailed report generated: $(COVERAGE_DIR)/index.html"; \
	else \
		echo "[coverage] gcovr not found. Falling back to gcov text report."; \
		if command -v $(GCOV) >/dev/null 2>&1; then \
			$(GCOV) -b -c $(BUILD_DIR)/CMakeFiles/child_lock_system.dir/src/*.gcda > $(COVERAGE_DIR)/gcov_report.txt; \
			mv -f *.gcov $(COVERAGE_DIR)/ 2>/dev/null || true; \
			echo "[coverage] fallback reports generated: $(COVERAGE_DIR)/gcov_report.txt and *.gcov"; \
		else \
			echo "[coverage] neither gcovr nor $(GCOV) is available."; \
			exit 1; \
		fi; \
	fi

complexity:
	$(LIZARD) -C 10 src include tests

loc:
	$(CLOC) src include tests

cppcheck:
	$(CPPCHECK) \
		--enable=warning,style,performance,portability \
		--error-exitcode=1 \
		--inline-suppr \
		--suppress=missingIncludeSystem \
		-Iinclude \
		src include

quality: cppcheck complexity loc

rebuild: clean test

clean:
	$(CMAKE) -E rm -rf $(BUILD_DIR)
