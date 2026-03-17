/**
 * @file child_lock.h
 * @brief Public interface for rear door latch release decision logic.
 * @details
 * This header provides the API for evaluating whether the ECU can
 * electronically release the rear door latch when the inside handle is pulled.
 *
 * Implemented Date: 2026-03-17
 * Implemented Time: 02:10:00 KST
 * Implemented Model: GPT-5 Codex
 * Version: v1.0
 */

#ifndef CHILD_LOCK_H
#define CHILD_LOCK_H

/* MSVC C mode in some toolsets does not provide <stdbool.h>. */
#if defined(_MSC_VER) && !defined(__clang__)
typedef unsigned char bool;
#define true 1
#define false 0
#else
#include <stdbool.h>
#endif

/**
 * @brief Evaluate whether the rear door latch can be released electronically.
 * @param detectRearDoorHandelPull True when the rear inside handle is pulled.
 * @param isMainPowerAvailable True when the main vehicle power is available.
 * @param isBackupPowerAvailable True when backup power is available.
 * @return True if the ECU should release the rear door latch, else false.
 *
 * @note
 * UC05 behavior:
 * - Main power available: release latch (normal operation).
 * - Main power unavailable: do not release latch electronically.
 * - Backup power may support emergency guidance only.
 */
bool ChildLock_EvaluateReleaseDoorLatch(bool detectRearDoorHandelPull,
                                        bool isMainPowerAvailable,
                                        bool isBackupPowerAvailable);

#endif /* CHILD_LOCK_H */
