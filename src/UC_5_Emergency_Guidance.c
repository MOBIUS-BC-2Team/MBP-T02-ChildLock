/**
 * @file UC_5_Emergency_Guidance.c
 * @brief UC05 emergency guidance logic for rear door electronic latch release.
 * @details
 * This module implements the UC05 decision path:
 * - Electronic latch release is allowed only with user request + main power.
 * - During main power loss, backup power supports emergency guidance only.
 *
 * Toolchain reference (Development.md):
 * - Compiler: GCC 11.4
 * - Language: C11
 * - Unit Test: GoogleTest 1.12.1
 * - Static Analysis: cppcheck 2.10
 *
 * Implemented Date: 2026-03-20
 * Implemented Time: 02:34:00 KST
 * Implemented Model: GPT-5 Codex
 * Version: v1.0
 */

#include "UC_5_Emergency_Guidance.h"

/**
 * @brief Evaluate whether ECU should release the rear door latch.
 * @param detect_rear_door_handle_pull Handle pull status from rear passenger.
 * @param is_main_power_available Main vehicle power status.
 * @param is_backup_power_available Backup power status.
 * @return True only when handle pull is requested and main power is available.
 */
bool UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch(
    bool detect_rear_door_handle_pull,
    bool is_main_power_available,
    bool is_backup_power_available)
{
    /*
     * Backup power drives emergency guidance HMI only.
     * This API returns electronic latch-release capability only.
     */
    (void)is_backup_power_available;

    if (!detect_rear_door_handle_pull)
    {
        return false;
    }

    return is_main_power_available;
}
