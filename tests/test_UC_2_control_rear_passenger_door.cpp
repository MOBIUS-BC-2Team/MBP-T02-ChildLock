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

// [TC-UC02-001] 테스트 목적: 모든 조건 정상 + 차일드락 OFF에서 도어 개방 허용 검증
TEST(ControlRearPassengerDoor, AllowsOpenWhenAllConditionsOkAndChildLockOff) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_ALLOWED);
}

// [TC-UC02-002] 테스트 목적: 실내 핸들 미조작 경계에서 도어 개방 차단 검증
TEST(ControlRearPassengerDoor, BlocksWhenHandleNotPulled) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(false, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC-UC02-003] 테스트 목적: 실내 핸들 조작 경계에서 다음 안전 조건 평가 진행 검증
TEST(ControlRearPassengerDoor, HandlePulledBoundaryWithAllSafeInputsAllowsOpen) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_ALLOWED);
}

// [TC-UC02-004] 테스트 목적: 안전 조건 비유효 시 도어 개방 차단 검증
TEST(ControlRearPassengerDoor, BlocksWhenSafetyInvalid) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, false, false, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC-UC02-005] 테스트 목적: 래치 센서 오류(FMEA) 시 도어 개방 차단 검증
TEST(ControlRearPassengerDoor, BlocksWhenLatchSensorError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, true, false, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC-UC02-006] 테스트 목적: ECU 오류(FMEA) 시 도어 개방 차단 검증
TEST(ControlRearPassengerDoor, BlocksWhenEcuError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, true, false, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC-UC02-007] 테스트 목적: 전원 이상(FMEA) 시 도어 개방 차단 검증
TEST(ControlRearPassengerDoor, BlocksWhenPowerError) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, true, false);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC-UC02-008] 테스트 목적: 차일드락 활성 상태에서 도어 개방 차단 검증
TEST(ControlRearPassengerDoor, BlocksWhenChildLockActive) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, true, false, false, false, true);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}

// [TC-UC02-009] 테스트 목적: 복수 고장 동시 입력 시 Fail-Safe 차단 유지 검증
TEST(ControlRearPassengerDoor, BlocksWhenMultipleFaultsOccurTogether) {
    const DoorOpenStatus status =
        EvaluateControlRearPassengerDoor(true, false, true, true, true, true);

    EXPECT_EQ(status, DOOR_OPEN_BLOCKED);
}
