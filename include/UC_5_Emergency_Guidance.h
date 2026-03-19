/**
 * @file UC_5_Emergency_Guidance.h
 * @brief Public interface for UC05 emergency guidance latch-release decision.
 * @details
 * This interface evaluates whether the ECU can release the rear door latch
 * electronically when the rear inside handle is pulled.
 *
 * Toolchain reference (Development.md):
 * - Compiler: GCC 11.4
 * - Language: C11 product code
 * - Unit Test: GoogleTest 1.12.1 (C++14 in CMake toolchain)
 * - Static Analysis: cppcheck 2.10
 *
 * Implemented Date: 2026-03-20
 * Implemented Time: 02:33:00 KST
 * Implemented Model: GPT-5 Codex
 * Version: v1.0
 */

#ifndef UC_5_EMERGENCY_GUIDANCE_H
#define UC_5_EMERGENCY_GUIDANCE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Evaluate whether the rear door latch can be released electronically.
 * @param detect_rear_door_handle_pull True when the rear inside handle is pulled.
 * @param is_main_power_available True when main vehicle power is available.
 * @param is_backup_power_available True when backup power is available.
 * @return True for handle-pull + main-power normal path, otherwise false.
 *
 * @note
 * UC05 behavior:
 * - Main power available: release latch (normal operation).
 * - Main power unavailable: do not release latch electronically.
 * - Backup power supports emergency guidance only.
 */
bool UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch(
    bool detect_rear_door_handle_pull,
    bool is_main_power_available,
    bool is_backup_power_available);

#ifdef __cplusplus
}
#endif

#endif /* UC_5_EMERGENCY_GUIDANCE_H */
