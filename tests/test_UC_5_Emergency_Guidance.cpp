/**
 * @file test_UC_5_Emergency_Guidance.cpp
 * @brief GoogleTest unit tests for UC05 emergency guidance latch logic.
 * @details
 * This test suite validates the UC05 decision output for normal and
 * power-loss conditions. Product code remains C11, while test code follows
 * C++14 with GoogleTest 1.12.1 from the project toolchain.
 *
 * Static analysis expectation:
 * - Product module is checked with cppcheck 2.10.
 *
 * Implemented Date: 2026-03-20
 * Implemented Time: 02:26:00 KST
 * Implemented Model: GPT-5 Codex
 * Version: v1.0
 */

#include <gtest/gtest.h>

extern "C"
{
#include "UC_5_Emergency_Guidance.h"
}

namespace
{

/**
 * @brief Wrapper to keep each test focused on behavior.
 * @param detect_rear_door_handle_pull True when the rear inside handle is pulled.
 * @param is_main_power_available True when main power is available.
 * @param is_backup_power_available True when backup power is available.
 * @return True if ECU should release latch electronically.
 */
bool EvaluateReleaseDoorLatch(bool detect_rear_door_handle_pull,
                              bool is_main_power_available,
                              bool is_backup_power_available)
{
    return UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch(
        detect_rear_door_handle_pull,
        is_main_power_available,
        is_backup_power_available);
}

} /* namespace */

// 테스트 목적: UC5EmergencyGuidance.KeepsLatchLockedWhenHandleIsIdle 동작 검증
TEST(UC5EmergencyGuidance, KeepsLatchLockedWhenHandleIsIdle)
{
    EXPECT_FALSE(EvaluateReleaseDoorLatch(false, true, true));
}

// 테스트 목적: UC5EmergencyGuidance.ReleasesLatchWhenHandlePulledAndMainPowerAvailable 동작 검증
TEST(UC5EmergencyGuidance, ReleasesLatchWhenHandlePulledAndMainPowerAvailable)
{
    EXPECT_TRUE(EvaluateReleaseDoorLatch(true, true, false));
}

// 테스트 목적: UC5EmergencyGuidance.MainPowerDominatesEvenWhenBackupAlsoAvailable 동작 검증
TEST(UC5EmergencyGuidance, MainPowerDominatesEvenWhenBackupAlsoAvailable)
{
    EXPECT_TRUE(EvaluateReleaseDoorLatch(true, true, true));
}

// 테스트 목적: UC5EmergencyGuidance.DoesNotReleaseWhenOnlyBackupPowerIsAvailable 동작 검증
TEST(UC5EmergencyGuidance, DoesNotReleaseWhenOnlyBackupPowerIsAvailable)
{
    EXPECT_FALSE(EvaluateReleaseDoorLatch(true, false, true));
}

// 테스트 목적: UC5EmergencyGuidance.DoesNotReleaseWhenNoPowerIsAvailable 동작 검증
TEST(UC5EmergencyGuidance, DoesNotReleaseWhenNoPowerIsAvailable)
{
    EXPECT_FALSE(EvaluateReleaseDoorLatch(true, false, false));
}

// 테스트 목적: UC5EmergencyGuidance.KeepsLatchLockedWhenNoRequestAndNoPower 동작 검증
TEST(UC5EmergencyGuidance, KeepsLatchLockedWhenNoRequestAndNoPower)
{
    EXPECT_FALSE(EvaluateReleaseDoorLatch(false, false, false));
}

// 테스트 목적: UC5EmergencyGuidance.BackupPowerToggleDoesNotChangeDecisionWhenMainPowerIsOn 동작 검증
TEST(UC5EmergencyGuidance, BackupPowerToggleDoesNotChangeDecisionWhenMainPowerIsOn)
{
    const bool result_without_backup = EvaluateReleaseDoorLatch(true, true, false);
    const bool result_with_backup = EvaluateReleaseDoorLatch(true, true, true);

    EXPECT_TRUE(result_without_backup);
    EXPECT_TRUE(result_with_backup);
    EXPECT_EQ(result_without_backup, result_with_backup);
}
