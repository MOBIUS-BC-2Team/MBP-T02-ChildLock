/**
 * @file UC_2_control_rear_passenger_door.c
 * @brief UC-2 based implementation for rear passenger door control.
 * @date 2026-03-15
 * @time 15:46:22
 * @model MBP-T02-ChildLock
 * @version v1.0
 *
 * Function Configuration:
 * - Input Value
 *   - is_handle_pulled: 0 (not pulled), 1 (pulled)
 *   - is_safety_valid: 0 (invalid), 1 (valid)
 *   - is_latch_sensor_error: 0 (normal), 1 (error)
 *   - is_ecu_error: 0 (normal), 1 (error)
 *   - is_power_error: 0 (normal), 1 (unstable/error)
 *   - is_child_lock_active: 0 (off), 1 (on)
 * - Output Value
 *   - doorOpenStatus: 0 (blocked), 1 (allowed)
 */

#include "UC_2_control_rear_passenger_door.h"

/**
 * @brief Determine whether the rear passenger door may open.
 *
 * @param is_handle_pulled True when the inside handle is pulled.
 * @param is_safety_valid True when vehicle safety conditions are valid.
 * @param is_latch_sensor_error True when the latch/unlock sensor has an error.
 * @param is_ecu_error True when ECU reports an error.
 * @param is_power_error True when vehicle power is unstable.
 * @param is_child_lock_active True when the child lock is active (ON).
 * @return DoorOpenStatus indicating allowed or blocked.
 */
DoorOpenStatus control_rear_passenger_door(bool is_handle_pulled,
                                          bool is_safety_valid,
                                          bool is_latch_sensor_error,
                                          bool is_ecu_error,
                                          bool is_power_error,
                                          bool is_child_lock_active) {
    if (!is_handle_pulled) {
        return DOOR_OPEN_BLOCKED;
    }

    /* Fail-safe: any unsafe or faulty condition blocks opening. */
    if (!is_safety_valid) {
        return DOOR_OPEN_BLOCKED;
    }

    if (is_latch_sensor_error) {
        return DOOR_OPEN_BLOCKED;
    }

    if (is_ecu_error) {
        return DOOR_OPEN_BLOCKED;
    }

    if (is_power_error) {
        return DOOR_OPEN_BLOCKED;
    }

    if (is_child_lock_active) {
        return DOOR_OPEN_BLOCKED;
    }

    return DOOR_OPEN_ALLOWED;
}
