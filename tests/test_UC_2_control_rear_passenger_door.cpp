/**
 * @file test_UC_2_control_rear_passenger_door.cpp
 * @brief UC-02 rear passenger door control unit tests.
 * @details
 * Test-case traceability is synchronized with doc/TestCase.md.
 */

#include "UC_2_control_rear_passenger_door.h"

#include <gtest/gtest.h>

namespace {

// Helper used to keep each test focused on one decision branch.
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

// [TC_UC02_001] Normal open path when all safety inputs are valid.
TEST(ControlRearPassengerDoor, AllowsOpenWhenAllConditionsOkAndChildLockOff) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_ALLOWED);
}

// [TC_UC02_007] No-handle-request path must remain blocked.
TEST(ControlRearPassengerDoor, BlocksWhenHandleNotPulled) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(false, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC_UC02_008] Handle boundary case (true) with safe conditions allows open.
TEST(ControlRearPassengerDoor, HandlePulledBoundaryWithAllSafeInputsAllowsOpen) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_ALLOWED);
}

// [TC_UC02_003] Safety-invalid input must block opening.
TEST(ControlRearPassengerDoor, BlocksWhenSafetyInvalid) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, false, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC_UC02_004] Latch sensor error (FMEA) must block opening.
TEST(ControlRearPassengerDoor, BlocksWhenLatchSensorError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, true, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC_UC02_005] ECU error (FMEA) must block opening.
TEST(ControlRearPassengerDoor, BlocksWhenEcuError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, true, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC_UC02_006] Power instability (FMEA) must block opening.
TEST(ControlRearPassengerDoor, BlocksWhenPowerError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, true, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC_UC02_002] Active child-lock must block opening.
TEST(ControlRearPassengerDoor, BlocksWhenChildLockActive) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, true);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC_UC02_009] Multiple simultaneous faults must still fail-safe to blocked.
TEST(ControlRearPassengerDoor, BlocksWhenMultipleFaultsOccurTogether) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, false, true, true, true, true);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}
