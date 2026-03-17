# Makefile for MBP-T02-ChildLock UC05 unit test and coverage workflow

CC ?= gcc-11
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -pedantic -Iinclude
LDFLAGS ?=
GCOV ?= gcov-11

TEST_BIN := tests/test_child_lock
COVERAGE_GCNO := $(TEST_BIN)-child_lock.gcno

.PHONY: all test clean coverage

all: test

$(TEST_BIN): tests/test_child_lock.c src/child_lock.c include/child_lock.h
	$(CC) $(CFLAGS) tests/test_child_lock.c src/child_lock.c $(LDFLAGS) -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

coverage: CFLAGS += --coverage -O0
coverage: LDFLAGS += --coverage
coverage: clean $(TEST_BIN)
	./$(TEST_BIN)
	$(GCOV) -b -c $(COVERAGE_GCNO)

clean:
	rm -f $(TEST_BIN) *.gcov
	rm -f src/*.gcda src/*.gcno tests/*.gcda tests/*.gcno
