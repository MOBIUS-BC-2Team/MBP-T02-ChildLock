/**
 * @file test_UC_1_child_lock_system_on_off.cpp
 * @brief GoogleTest unit tests for the UC01 child lock toggle service.
 * @details
 * Implemented: 2026-03-17 14:05 KST
 * Model: GPT-5 Codex
 * Version: v1.0
 *
 * Product code remains in C11, while the unit tests follow the repository
 * guidance and are implemented in C++14 with GoogleTest 1.12.1.
 */

#include <cstring>

#include <gtest/gtest.h>

extern "C"
{
#include "../include/UC_1_child_lock_system_on_off.h"
}

namespace
{

/**
 * @brief Create a valid default UC01 request for test reuse.
 * @return Initialized request object with a success path baseline.
 */
ChildLockRequest MakeValidRequest()
{
    ChildLockRequest request;

    ChildLockSystem_InitRequest(&request);
    request.door_id = CHILD_LOCK_DOOR_RL;
    request.current_child_lock_state = CHILD_LOCK_STATE_OFF;
    request.acc_on = true;
    request.speed_kph = 0U;
    request.crash_override = false;
    request.power_loss = false;
    request.sea_status = CHILD_LOCK_SEA_SAFE;
    request.driver_confirmed_again = false;
    request.apply_result = CHILD_LOCK_APPLY_SUCCESS;

    return request;
}

// 테스트 목적: ChildLockSystemHelpers.InitRequestSetsSafeDefaults 동작 검증
TEST(ChildLockSystemHelpers, InitRequestSetsSafeDefaults)
{
    ChildLockRequest request;

    std::memset(&request, 0xFF, sizeof(request));
    ChildLockSystem_InitRequest(&request);

    EXPECT_EQ(CHILD_LOCK_DOOR_RL, request.door_id);
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, request.current_child_lock_state);
    EXPECT_FALSE(request.acc_on);
    EXPECT_EQ(0U, request.speed_kph);
    EXPECT_FALSE(request.crash_override);
    EXPECT_FALSE(request.power_loss);
    EXPECT_EQ(CHILD_LOCK_SEA_SAFE, request.sea_status);
    EXPECT_FALSE(request.driver_confirmed_again);
    EXPECT_EQ(CHILD_LOCK_APPLY_SUCCESS, request.apply_result);
}

// 테스트 목적: ChildLockSystemHelpers.InitResponseKeepsCurrentState 동작 검증
TEST(ChildLockSystemHelpers, InitResponseKeepsCurrentState)
{
    ChildLockResponse response;

    std::memset(&response, 0xFF, sizeof(response));
    ChildLockSystem_InitResponse(&response, CHILD_LOCK_STATE_ON);

    EXPECT_EQ(CHILD_LOCK_STATE_ON, response.new_child_lock_state);
    EXPECT_EQ(CHILD_LOCK_STATE_ON, response.target_child_lock_state);
    EXPECT_EQ(nullptr, response.display_message);
    EXPECT_EQ(CHILD_LOCK_RESULT_INVALID_REQUEST, response.result_status);
    EXPECT_FALSE(response.fault_log_needed);
    EXPECT_FALSE(response.command_sent);
    EXPECT_FALSE(response.status_update_needed);
}

// 테스트 목적: ChildLockSystemHelpers.DoorValidationAcceptsRearDoors 동작 검증
TEST(ChildLockSystemHelpers, DoorValidationAcceptsRearDoors)
{
    EXPECT_TRUE(ChildLockSystem_IsDoorIdValid(CHILD_LOCK_DOOR_RL));
    EXPECT_TRUE(ChildLockSystem_IsDoorIdValid(CHILD_LOCK_DOOR_RR));
}

// 테스트 목적: ChildLockSystemHelpers.DoorValidationRejectsInvalidDoor 동작 검증
TEST(ChildLockSystemHelpers, DoorValidationRejectsInvalidDoor)
{
    EXPECT_FALSE(ChildLockSystem_IsDoorIdValid(CHILD_LOCK_DOOR_INVALID));
}

// 테스트 목적: ChildLockSystemHelpers.ToggleStateTurnsOffToOn 동작 검증
TEST(ChildLockSystemHelpers, ToggleStateTurnsOffToOn)
{
    EXPECT_EQ(CHILD_LOCK_STATE_ON,
              ChildLockSystem_ToggleState(CHILD_LOCK_STATE_OFF));
}

// 테스트 목적: ChildLockSystemHelpers.ToggleStateTurnsOnToOff 동작 검증
TEST(ChildLockSystemHelpers, ToggleStateTurnsOnToOff)
{
    EXPECT_EQ(CHILD_LOCK_STATE_OFF,
              ChildLockSystem_ToggleState(CHILD_LOCK_STATE_ON));
}

// 테스트 목적: ChildLockSystemProcess.RejectsNullRequest 동작 검증
TEST(ChildLockSystemProcess, RejectsNullRequest)
{
    ChildLockResponse response;

    ChildLockSystem_InitResponse(&response, CHILD_LOCK_STATE_OFF);

    EXPECT_EQ(CHILD_LOCK_RESULT_INVALID_REQUEST,
              ChildLockSystem_ProcessRequest(nullptr, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_FALSE(response.command_sent);
}

// 테스트 목적: ChildLockSystemProcess.RejectsNullResponse 동작 검증
TEST(ChildLockSystemProcess, RejectsNullResponse)
{
    const ChildLockRequest request = MakeValidRequest();

    EXPECT_EQ(CHILD_LOCK_RESULT_INVALID_REQUEST,
              ChildLockSystem_ProcessRequest(&request, nullptr));
}

// 테스트 목적: ChildLockSystemProcess.RejectsInvalidDoor 동작 검증
TEST(ChildLockSystemProcess, RejectsInvalidDoor)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.door_id = CHILD_LOCK_DOOR_INVALID;

    EXPECT_EQ(CHILD_LOCK_RESULT_INVALID_REQUEST,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_FALSE(response.command_sent);
    EXPECT_STREQ("지원하지 않는 도어입니다.", response.display_message);
}

// 테스트 목적: ChildLockSystemProcess.BlocksAccOff 동작 검증
TEST(ChildLockSystemProcess, BlocksAccOff)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.acc_on = false;

    EXPECT_EQ(CHILD_LOCK_RESULT_BLOCKED_ACC_OFF,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_FALSE(response.command_sent);
    EXPECT_STREQ("ACC OFF: 설정 변경 불가", response.display_message);
}

// 테스트 목적: ChildLockSystemProcess.BlocksWhenSpeedIsNonZero 동작 검증
TEST(ChildLockSystemProcess, BlocksWhenSpeedIsNonZero)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.speed_kph = 5U;

    EXPECT_EQ(CHILD_LOCK_RESULT_BLOCKED_SPEED,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_FALSE(response.command_sent);
    EXPECT_STREQ("정차 후 변경 가능", response.display_message);
}

// 테스트 목적: ChildLockSystemProcess.BlocksWhenSpeedIsOneBoundary 동작 검증
TEST(ChildLockSystemProcess, BlocksWhenSpeedIsOneBoundary)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.speed_kph = 1U;

    EXPECT_EQ(CHILD_LOCK_RESULT_BLOCKED_SPEED,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_FALSE(response.command_sent);
    EXPECT_STREQ("정차 후 변경 가능", response.display_message);
}

// 테스트 목적: ChildLockSystemProcess.BlocksWhenSpeedIsUint16MaxBoundary 동작 검증
TEST(ChildLockSystemProcess, BlocksWhenSpeedIsUint16MaxBoundary)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.speed_kph = 65535U;

    EXPECT_EQ(CHILD_LOCK_RESULT_BLOCKED_SPEED,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_FALSE(response.command_sent);
}

// 테스트 목적: ChildLockSystemProcess.BlocksOnCrashOverride 동작 검증
TEST(ChildLockSystemProcess, BlocksOnCrashOverride)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.crash_override = true;

    EXPECT_EQ(CHILD_LOCK_RESULT_BLOCKED_CRASH,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_FALSE(response.command_sent);
    EXPECT_STREQ("사고 감지: 설정 변경 불가 (강제 OFF 우선)",
                 response.display_message);
}

// 테스트 목적: ChildLockSystemProcess.BlocksOnPowerLoss 동작 검증
TEST(ChildLockSystemProcess, BlocksOnPowerLoss)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.power_loss = true;

    EXPECT_EQ(CHILD_LOCK_RESULT_BLOCKED_POWER,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_FALSE(response.command_sent);
    EXPECT_STREQ("전원 불안정: 설정 변경 불가", response.display_message);
}

// 테스트 목적: ChildLockSystemProcess.SucceedsForSafeLockEnable 동작 검증
TEST(ChildLockSystemProcess, SucceedsForSafeLockEnable)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    EXPECT_EQ(CHILD_LOCK_RESULT_OK,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_ON, response.new_child_lock_state);
    EXPECT_EQ(CHILD_LOCK_STATE_ON, response.target_child_lock_state);
    EXPECT_TRUE(response.command_sent);
    EXPECT_TRUE(response.status_update_needed);
    EXPECT_FALSE(response.fault_log_needed);
    EXPECT_EQ(nullptr, response.display_message);
}

// 테스트 목적: ChildLockSystemProcess.RequiresReconfirmationForRiskyUnlock 동작 검증
TEST(ChildLockSystemProcess, RequiresReconfirmationForRiskyUnlock)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.current_child_lock_state = CHILD_LOCK_STATE_ON;
    request.sea_status = CHILD_LOCK_SEA_RISK;
    request.driver_confirmed_again = false;

    EXPECT_EQ(CHILD_LOCK_RESULT_CONFIRMATION_REQUIRED,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_ON, response.new_child_lock_state);
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.target_child_lock_state);
    EXPECT_FALSE(response.command_sent);
    EXPECT_FALSE(response.status_update_needed);
    EXPECT_STREQ("후석 접근 감지: 재조작시 해제", response.display_message);
}

// 테스트 목적: ChildLockSystemProcess.AllowsRiskyUnlockAfterReconfirm 동작 검증
TEST(ChildLockSystemProcess, AllowsRiskyUnlockAfterReconfirm)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.current_child_lock_state = CHILD_LOCK_STATE_ON;
    request.sea_status = CHILD_LOCK_SEA_RISK;
    request.driver_confirmed_again = true;

    EXPECT_EQ(CHILD_LOCK_RESULT_OK,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.target_child_lock_state);
    EXPECT_TRUE(response.command_sent);
    EXPECT_TRUE(response.status_update_needed);
}

// 테스트 목적: ChildLockSystemProcess.AllowsRiskStateWhenTargetIsOn 동작 검증
TEST(ChildLockSystemProcess, AllowsRiskStateWhenTargetIsOn)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.current_child_lock_state = CHILD_LOCK_STATE_OFF;
    request.sea_status = CHILD_LOCK_SEA_RISK;

    EXPECT_EQ(CHILD_LOCK_RESULT_OK,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_ON, response.new_child_lock_state);
    EXPECT_TRUE(response.command_sent);
}

// 테스트 목적: ChildLockSystemProcess.ReportsApplyFail 동작 검증
TEST(ChildLockSystemProcess, ReportsApplyFail)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.apply_result = CHILD_LOCK_APPLY_FAIL;

    EXPECT_EQ(CHILD_LOCK_RESULT_APPLY_FAILED,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_EQ(CHILD_LOCK_STATE_ON, response.target_child_lock_state);
    EXPECT_TRUE(response.command_sent);
    EXPECT_FALSE(response.status_update_needed);
    EXPECT_FALSE(response.fault_log_needed);
    EXPECT_STREQ("설정 실패 (도어 장치 오류)", response.display_message);
}

// 테스트 목적: ChildLockSystemProcess.ReportsApplyTimeout 동작 검증
TEST(ChildLockSystemProcess, ReportsApplyTimeout)
{
    ChildLockRequest request = MakeValidRequest();
    ChildLockResponse response;

    request.apply_result = CHILD_LOCK_APPLY_TIMEOUT;

    EXPECT_EQ(CHILD_LOCK_RESULT_APPLY_TIMEOUT,
              ChildLockSystem_ProcessRequest(&request, &response));
    EXPECT_EQ(CHILD_LOCK_STATE_OFF, response.new_child_lock_state);
    EXPECT_EQ(CHILD_LOCK_STATE_ON, response.target_child_lock_state);
    EXPECT_TRUE(response.command_sent);
    EXPECT_FALSE(response.status_update_needed);
    EXPECT_TRUE(response.fault_log_needed);
    EXPECT_STREQ("도어 응답 없음 설정 실패", response.display_message);
}

}  // namespace
