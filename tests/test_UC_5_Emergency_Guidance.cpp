/**
 * @file test_UC_5_Emergency_Guidance.cpp
 * @brief UC-05 emergency guidance latch-decision unit tests.
 * @details
 * Test-case traceability is synchronized with doc/TestCase.md.
 */

#include <gtest/gtest.h>

extern "C"
{
#include "UC_5_Emergency_Guidance.h"
}

namespace
{

/**
 * @brief Small wrapper to keep test bodies focused on expected behavior.
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

// [TC_UC05_004] Handle idle must keep the electronic latch locked.
TEST(UC5EmergencyGuidance, KeepsLatchLockedWhenHandleIsIdle)
{
    EXPECT_FALSE(EvaluateReleaseDoorLatch(false, true, true));
}

// [TC_UC05_001] Handle request + main power available allows electronic release.
TEST(UC5EmergencyGuidance, ReleasesLatchWhenHandlePulledAndMainPowerAvailable)
{
    EXPECT_TRUE(EvaluateReleaseDoorLatch(true, true, false));
}

// [TC_NA] Regression guard: main power path remains true even with backup available.
TEST(UC5EmergencyGuidance, MainPowerDominatesEvenWhenBackupAlsoAvailable)
{
    EXPECT_TRUE(EvaluateReleaseDoorLatch(true, true, true));
}

// [TC_UC05_002] Main power loss + backup only does not allow electronic release.
TEST(UC5EmergencyGuidance, DoesNotReleaseWhenOnlyBackupPowerIsAvailable)
{
    EXPECT_FALSE(EvaluateReleaseDoorLatch(true, false, true));
}

// [TC_UC05_003] Total power loss does not allow electronic release.
TEST(UC5EmergencyGuidance, DoesNotReleaseWhenNoPowerIsAvailable)
{
    EXPECT_FALSE(EvaluateReleaseDoorLatch(true, false, false));
}

// [TC_UC05_004] No request branch remains false regardless of power state.
TEST(UC5EmergencyGuidance, KeepsLatchLockedWhenNoRequestAndNoPower)
{
    EXPECT_FALSE(EvaluateReleaseDoorLatch(false, false, false));
}

// [TC_NA] Regression guard: toggling backup power does not change main-power decision.
TEST(UC5EmergencyGuidance, BackupPowerToggleDoesNotChangeDecisionWhenMainPowerIsOn)
{
    const bool result_without_backup = EvaluateReleaseDoorLatch(true, true, false);
    const bool result_with_backup = EvaluateReleaseDoorLatch(true, true, true);

    EXPECT_TRUE(result_without_backup);
    EXPECT_TRUE(result_with_backup);
    EXPECT_EQ(result_without_backup, result_with_backup);
}
