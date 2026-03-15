/**
 * @file safe_exit_assist.c
 * @brief 하차 안전 보조(SEA) 기능 로직 구현부
 * @date 2026-03-15
 * @time 13:27
 * @model UC-03 Safe Exit Assist
 * @version v1.0
 * * @details FlowChart 기반의 조건 분기점 및 예외 처리(Fail-Safe) 적용
 */

#include "safe_exit_assist.h"

#define RISK_SPEED_THRESHOLD (6.0f) /* 위험 감지 임계 속도 (km/h) */

/**
 * @brief 하차 안전 보조 제어 로직 수행
 * @param[in] unlock_request 잠금 해제 요청
 * @param[in] radar_object_speed 접근 객체 속도
 * @param[in] radar_status 레이더 상태
 * @return SeaOutput_t 시스템 최종 제어 명령
 */
SeaOutput_t Execute_Safe_Exit_Assist(bool unlock_request, float radar_object_speed, RadarStatus_e radar_status) {
    SeaOutput_t output = {MAINTAIN_LOCK, WARNING_OFF}; // 기본 안전 상태로 초기화

    // 해제 요청이 없는 경우 로직 종료 (안전 상태 유지)
    if (!unlock_request) {
        return output;
    }

    // 예외 처리: 센서 오류 시 Fail-Safe 모드 진입
    if (radar_status != RADAR_NORMAL) {
        output.door_cmd = MAINTAIN_LOCK;
        output.hmi_cmd = ERROR_POPUP;
        return output;
    }

    // 정상 작동: 위험 속도 기준 초과 여부 판단
    if (radar_object_speed >= RISK_SPEED_THRESHOLD) {
        // 위험 감지 시 잠금 유지 및 경고 발생
        output.door_cmd = MAINTAIN_LOCK;
        output.hmi_cmd = WARNING_ON;
    } else {
        // 안전 확보 시 도어 잠금 해제 허용
        output.door_cmd = RELEASE_LOCK;
        output.hmi_cmd = WARNING_OFF;
    }

    return output;
}