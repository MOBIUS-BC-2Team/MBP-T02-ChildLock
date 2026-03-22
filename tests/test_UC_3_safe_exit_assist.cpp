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

// 1. 정상 흐름: 해제 요청이 없을 때는 잠금 유지, 경고 없음
TEST(SafeExitAssistTest, NoUnlockRequest_MaintainLock) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(false, 0.0f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}

// 2. 정상 해제: 위험 객체가 없고(속도 6km/h 미만), 센서가 정상일 때
TEST(SafeExitAssistTest, SafeToExit_ReleaseLock) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 3.5f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, RELEASE_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}

// 2-1. 경계값: 임계 속도 바로 아래에서는 잠금 해제 허용
TEST(SafeExitAssistTest, BoundaryBelowThreshold_ReleaseLock) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 5.9f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, RELEASE_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}

// 2-2. 경계값: 임계 속도와 동일하면 위험으로 판단
TEST(SafeExitAssistTest, BoundaryAtThreshold_MaintainLockAndWarn) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 6.0f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_ON);
}

// 2-3. 경계값: 임계 속도 바로 위에서는 잠금 유지
TEST(SafeExitAssistTest, BoundaryAboveThreshold_MaintainLockAndWarn) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 6.1f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_ON);
}

// 3. 위험 감지(SEA 작동): 6km/h 이상으로 접근하는 객체 감지 시 잠금 유지
TEST(SafeExitAssistTest, RiskDetected_MaintainLockAndWarn) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 6.5f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_ON);
}

// 4. 예외 처리(FMEA): 센서 타임아웃 발생 시 강제 잠금(Fail-Safe)
TEST(SafeExitAssistTest, SensorTimeout_FailSafe) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 0.0f, RADAR_TIMEOUT);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, ERROR_POPUP);
}

// 5. 예외 처리(FMEA): 센서 에러 발생 시 강제 잠금(Fail-Safe)
TEST(SafeExitAssistTest, SensorError_FailSafe) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, 10.0f, RADAR_ERROR);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, ERROR_POPUP);
}

// 6. 해제 요청이 없으면 센서 상태와 무관하게 초기 안전 상태 유지
TEST(SafeExitAssistTest, NoUnlockRequest_PrioritizedOverRadarError) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(false, 0.0f, RADAR_TIMEOUT);
    EXPECT_EQ(result.door_cmd, MAINTAIN_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}

// 7. 비정상 입력(음수 속도) 시 현재 구현 기준으로는 안전 구간 처리
TEST(SafeExitAssistTest, NegativeSpeed_CurrentLogicReleasesLock) {
    SeaOutput_t result = Execute_Safe_Exit_Assist(true, -1.0f, RADAR_NORMAL);
    EXPECT_EQ(result.door_cmd, RELEASE_LOCK);
    EXPECT_EQ(result.hmi_cmd, WARNING_OFF);
}
