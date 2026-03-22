/**
 * @file test_UC_2_control_rear_passenger_door.cpp
 * @brief Unit tests for UC-2 based rear passenger door control logic.
 * @date 2026-03-15
 * @time 15:46:22
 * @model MBP-T02-ChildLock
 * @version v1.0
 */

#include "UC_2_control_rear_passenger_door.h"

#include <gtest/gtest.h>

namespace {

// Helper to keep test bodies concise and focused on expectations.
// Note: inputs are modeled as booleans to match the UC-2 flowchart decisions.
static DoorOpenStatus EvaluateControlRearPassengerDoor(bool is_handle_pulled,
                                                       bool is_safety_valid,
                                                       bool is_latch_sensor_error,
                                                       bool is_ecu_error,
                                                       bool is_power_error,
                                                       bool is_child_lock_active) {
    return control_rear_passenger_door(is_handle_pulled,
                                       is_safety_valid,
                                       is_latch_sensor_error,
                                       is_ecu_error,
                                       is_power_error,
                                       is_child_lock_active);
}

}  // namespace

// Each test toggles one condition to ensure independent blocking behavior.
// Normal case: all conditions valid and child lock off should allow opening.
TEST(ControlRearPassengerDoor, AllowsOpenWhenAllConditionsOkAndChildLockOff) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_ALLOWED);
}

// Edge case: handle not pulled always blocks.
TEST(ControlRearPassengerDoor, BlocksWhenHandleNotPulled) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(false, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// Boundary case: handle pulled allows evaluation to continue.
TEST(ControlRearPassengerDoor, HandlePulledBoundaryWithAllSafeInputsAllowsOpen) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_ALLOWED);
}

// Safety invalid must block even if handle is pulled.
TEST(ControlRearPassengerDoor, BlocksWhenSafetyInvalid) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, false, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// Latch sensor error should block.
TEST(ControlRearPassengerDoor, BlocksWhenLatchSensorError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, true, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// ECU error should block.
TEST(ControlRearPassengerDoor, BlocksWhenEcuError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, true, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// Power instability should block.
TEST(ControlRearPassengerDoor, BlocksWhenPowerError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, true, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// Child lock active should block.
TEST(ControlRearPassengerDoor, BlocksWhenChildLockActive) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, true);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// FMEA-oriented stress case: multiple concurrent faults must remain blocked.
TEST(ControlRearPassengerDoor, BlocksWhenMultipleFaultsOccurTogether) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, false, true, true, true, true);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}
