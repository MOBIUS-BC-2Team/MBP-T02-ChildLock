/**
 * @file test_control_rear_passenger_door.cpp
 * @brief Unit tests for UC-2 based rear passenger door control logic.
 * @date 2026-03-15
 * @time 15:46:22
 * @model MBP-T02-ChildLock
 * @version v1.0
 */

#include "control_rear_passenger_door.h"

#include <gtest/gtest.h>

namespace {

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

TEST(ControlRearPassengerDoor, AllowsOpenWhenAllConditionsOkAndChildLockOff) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_ALLOWED);
}

TEST(ControlRearPassengerDoor, BlocksWhenHandleNotPulled) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(false, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

TEST(ControlRearPassengerDoor, BlocksWhenSafetyInvalid) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, false, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

TEST(ControlRearPassengerDoor, BlocksWhenLatchSensorError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, true, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

TEST(ControlRearPassengerDoor, BlocksWhenEcuError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, true, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

TEST(ControlRearPassengerDoor, BlocksWhenPowerError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, true, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

TEST(ControlRearPassengerDoor, BlocksWhenChildLockActive) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, true);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}
