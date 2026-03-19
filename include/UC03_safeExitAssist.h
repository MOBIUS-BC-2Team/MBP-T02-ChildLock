/**
 * @file safe_exit_assist.h
 * @brief 하차 안전 보조(SEA) 기능 정의 헤더
 * @date 2026-03-15
 * @time 13:27
 * @model UC-03 Safe Exit Assist
 * @version v1.0
 * * @details 차량 정차 후 탑승자 하차 시, 후측방 레이더 데이터를 분석하여 
 * 접근 차량(6km/h 이상)이 있거나 센서 통신 오류 발생 시 
 * 도어 잠금을 강제로 유지하고 안전 경고를 발생시킨다.
 */

#ifndef SAFE_EXIT_ASSIST_H
#define SAFE_EXIT_ASSIST_H

#include <stdbool.h>

/**
 * @enum RadarStatus_e
 * @brief 레이더 센서 상태
 */
typedef enum {
    RADAR_NORMAL,   /**< 정상 작동 */
    RADAR_TIMEOUT,  /**< 통신 지연/응답 없음 */
    RADAR_ERROR     /**< 센서 오류 */
} RadarStatus_e;

/**
 * @enum DoorLockCmd_e
 * @brief 도어 래치 제어 명령
 */
typedef enum {
    MAINTAIN_LOCK,  /**< 잠금 유지 (안전 상태) */
    RELEASE_LOCK    /**< 잠금 해제 */
} DoorLockCmd_e;

/**
 * @enum HmiWarningCmd_e
 * @brief HMI 경고 알림 명령
 */
typedef enum {
    WARNING_OFF,    /**< 경고 없음 */
    WARNING_ON,     /**< 위험 감지 경고 발생 */
    ERROR_POPUP     /**< 시스템 에러 팝업 발생 */
} HmiWarningCmd_e;

/**
 * @struct SeaOutput_t
 * @brief SEA 시스템 제어 출력 구조체
 */
typedef struct {
    DoorLockCmd_e door_cmd;
    HmiWarningCmd_e hmi_cmd;
} SeaOutput_t;

/**
 * @brief 하차 안전 보조 제어 판단 함수
 * @param[in] unlock_request 탑승자의 도어 잠금 해제 요청 신호 (true/false)
 * @param[in] radar_object_speed 후측방 접근 객체의 속도 (km/h)
 * @param[in] radar_status 레이더 센서 정상 작동 여부 상태값
 * @return SeaOutput_t 도어 래치 명령 및 HMI 경고 명령을 포함한 구조체
 */
SeaOutput_t Execute_Safe_Exit_Assist(bool unlock_request, float radar_object_speed, RadarStatus_e radar_status);

#endif /* SAFE_EXIT_ASSIST_H */