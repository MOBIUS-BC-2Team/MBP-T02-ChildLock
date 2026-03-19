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
