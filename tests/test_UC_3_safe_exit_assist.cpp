/**
 * @file test_UC_3_safe_exit_assist.cpp
 * @brief 하차 안전 보조(SEA) 기능 단위 테스트 (GoogleTest)
 * @date 2026-03-15
 * @time 13:27
 * @model UC-03 Safe Exit Assist
 * @version v1.0
 */

#include <gtest/gtest.h>

extern "C" {
    #include "UC_3_safe_exit_assist.h"
}

// [TC-UC03-001] 테스트 목적: 해제 요청이 없을 때 기본 안전 상태 유지 검증
TEST(SafeExitAssistTest, NoUnlockRequest_MaintainLock) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(false, 0.0f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}

// [TC-UC03-002] 테스트 목적: 안전 속도 구간(6.0 미만)에서 잠금 해제 허용 검증
TEST(SafeExitAssistTest, SafeToExit_ReleaseLock) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 3.5f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, RELEASE_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}

// [TC-UC03-004] 테스트 목적: 속도 임계값 바로 아래(5.9) 경계 동작 검증
TEST(SafeExitAssistTest, BoundaryBelowThreshold_ReleaseLock) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 5.9f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, RELEASE_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}

// [TC-UC03-005] 테스트 목적: 속도 임계값 정확히 일치(6.0) 시 위험 판단 검증
TEST(SafeExitAssistTest, BoundaryAtThreshold_MaintainLockAndWarn) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 6.0f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_ON);
}

// [TC-UC03-006] 테스트 목적: 속도 임계값 바로 위(6.1) 경계 동작 검증
TEST(SafeExitAssistTest, BoundaryAboveThreshold_MaintainLockAndWarn) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 6.1f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_ON);
}

// [TC-UC03-003] 테스트 목적: 위험 속도 구간(6.0 이상)에서 잠금 유지 및 경고 검증
TEST(SafeExitAssistTest, RiskDetected_MaintainLockAndWarn) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 6.5f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_ON);
}

// [TC-UC03-007] 테스트 목적: 센서 타임아웃(FMEA) 시 Fail-Safe 강제 잠금 검증
TEST(SafeExitAssistTest, SensorTimeout_FailSafe) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 0.0f, RADAR_TIMEOUT);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, ERROR_POPUP);
}

// [TC-UC03-008] 테스트 목적: 센서 오류(FMEA) 시 Fail-Safe 강제 잠금 검증
TEST(SafeExitAssistTest, SensorError_FailSafe) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 10.0f, RADAR_ERROR);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, ERROR_POPUP);
}

// [TC-UC03-009] 테스트 목적: 해제 요청 없음 분기가 센서 오류보다 우선되는지 검증
TEST(SafeExitAssistTest, NoUnlockRequest_PrioritizedOverRadarError) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(false, 0.0f, RADAR_TIMEOUT);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}

// [TC-UC03-010] 테스트 목적: 음수 속도 비정상 입력 처리(현재 로직) 검증
TEST(SafeExitAssistTest, NegativeSpeed_CurrentLogicReleasesLock) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, -1.0f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, RELEASE_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}
