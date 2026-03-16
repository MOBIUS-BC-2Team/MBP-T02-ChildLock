/**
 * @file test_child_lock.c
 * @brief Unit tests for rear door latch release decision logic.
 * @details
 * The test vectors cover normal and edge conditions from UC05:
 * handle idle, normal power path, and power-loss fallback paths.
 *
 * Implemented Date: 2026-03-17
 * Implemented Time: 02:11:00 KST
 * Implemented Model: GPT-5 Codex
 * Version: v1.0
 */

#include <stddef.h>
#include <stdio.h>

#include "child_lock.h"

/**
 * @brief Input/output set for one test case.
 */
typedef struct
{
    bool detectRearDoorHandelPull;
    bool isMainPowerAvailable;
    bool isBackupPowerAvailable;
    bool expectedReleaseDoorLatch;
    const char *name;
} ChildLockTestCase;

/**
 * @brief Execute one vector and print a readable failure message.
 * @param testCase Pointer to one immutable vector.
 * @return True if expected and actual outputs are equal.
 */
static bool runSingleCase(const ChildLockTestCase *testCase)
{
    const bool actual = ChildLock_EvaluateReleaseDoorLatch(
        testCase->detectRearDoorHandelPull,
        testCase->isMainPowerAvailable,
        testCase->isBackupPowerAvailable);

    if (actual == testCase->expectedReleaseDoorLatch)
    {
        return true;
    }

    printf("FAIL: %s expected=%d actual=%d\n",
           testCase->name,
           testCase->expectedReleaseDoorLatch,
           actual);
    return false;
}

/**
 * @brief Test runner entry point.
 * @return 0 when all test vectors pass, 1 otherwise.
 */
int main(void)
{
    const ChildLockTestCase cases[] =
    {
        {false, true, true, false, "Handle idle, keep latch locked"},
        {false, false, false, false, "No request with full power loss"},
        {true, true, false, true, "Normal path: main power available"},
        {true, true, true, true, "Main power dominates even with backup"},
        {true, false, true, false, "Backup only: guidance, no latch release"},
        {true, false, false, false, "No power: no electronic latch release"}
    };

    bool allPassed = true;
    size_t i = 0U;

    for (i = 0U; i < (sizeof(cases) / sizeof(cases[0])); ++i)
    {
        if (!runSingleCase(&cases[i]))
        {
            allPassed = false;
        }
    }

    if (!allPassed)
    {
        puts("Test result: FAILED");
        return 1;
    }

    puts("Test result: PASSED");
    return 0;
}
