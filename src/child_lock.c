/**
 * @file child_lock.c
 * @brief Rear door latch release decision logic for UC05.
 * @details
 * This module implements the electronic decision path from the UC05 diagrams.
 * Main power allows normal latch release. During main power loss, backup power
 * supports emergency guidance only and does not trigger electronic latch release.
 *
 * Implemented Date: 2026-03-17
 * Implemented Time: 02:14:00 KST
 * Implemented Model: GPT-5 Codex
 * Version: v1.0
 */

#include "child_lock.h"

/**
 * @brief Evaluate whether the ECU should release the rear door latch.
 * @param detectRearDoorHandelPull Handle pull status from rear seat passenger.
 * @param isMainPowerAvailable Main vehicle power status.
 * @param isBackupPowerAvailable Backup power status.
 * @return True only for the normal path where handle is pulled and main power exists.
 */
bool ChildLock_EvaluateReleaseDoorLatch(bool detectRearDoorHandelPull,
                                        bool isMainPowerAvailable,
                                        bool isBackupPowerAvailable)
{
    /* Edge case: no user request means no latch actuation. */
    if (!detectRearDoorHandelPull)
    {
        return false;
    }

    /* Normal path from the flowchart: open door electronically. */
    if (isMainPowerAvailable)
    {
        return true;
    }

    /*
     * Power-loss path: backup power may present guidance to use
     * the mechanical emergency lever, but ECU latch actuation stays off.
     */
    if (isBackupPowerAvailable)
    {
        return false;
    }

    /* No main or backup power: no electronic release is possible. */
    return false;
}
