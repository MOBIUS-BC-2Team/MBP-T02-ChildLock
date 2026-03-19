/**
 * @file UC_1_child_lock_system_on_off.c
 * @brief UC01 implementation for electronic child lock ON/OFF control.
 * @details
 * Implemented: 2026-03-17 13:30 KST
 * Model: GPT-5 Codex
 * Version: v1.0
 *
 * The module models BCM decision logic for a driver child-lock toggle request.
 * It validates safety preconditions, derives the target state, handles risky
 * unlock reconfirmation, and processes ECU apply feedback.
 */

#include <stddef.h>

#include "../include/UC_1_child_lock_system_on_off.h"

/**
 * @brief HMI message shown when an unsupported door is requested.
 */
static const char *const CHILD_LOCK_MSG_INVALID_DOOR =
    "지원하지 않는 도어입니다.";

/**
 * @brief HMI message shown when ACC is OFF.
 */
static const char *const CHILD_LOCK_MSG_ACC_OFF =
    "ACC OFF: 설정 변경 불가";

/**
 * @brief HMI message shown when the vehicle is moving.
 */
static const char *const CHILD_LOCK_MSG_SPEED =
    "정차 후 변경 가능";

/**
 * @brief HMI message shown when crash override is active.
 */
static const char *const CHILD_LOCK_MSG_CRASH =
    "사고 감지: 설정 변경 불가 (강제 OFF 우선)";

/**
 * @brief HMI message shown when power is unstable.
 */
static const char *const CHILD_LOCK_MSG_POWER =
    "전원 불안정: 설정 변경 불가";

/**
 * @brief HMI message shown when risky unlock reconfirmation is required.
 */
static const char *const CHILD_LOCK_MSG_RECONFIRM =
    "후석 접근 감지: 재조작시 해제";

/**
 * @brief HMI message shown when door ECU reports a device failure.
 */
static const char *const CHILD_LOCK_MSG_APPLY_FAIL =
    "설정 실패 (도어 장치 오류)";

/**
 * @brief HMI message shown when door ECU does not respond.
 */
static const char *const CHILD_LOCK_MSG_APPLY_TIMEOUT =
    "도어 응답 없음 설정 실패";

/**
 * @brief Initialize a request with safe defaults.
 * @param request Request object to initialize. NULL is ignored.
 * @return None.
 */
void ChildLockSystem_InitRequest(ChildLockRequest *request)
{
    if (request == NULL)
    {
        return;
    }

    request->door_id = CHILD_LOCK_DOOR_RL;
    request->current_child_lock_state = CHILD_LOCK_STATE_OFF;
    request->acc_on = false;
    request->speed_kph = 0U;
    request->crash_override = false;
    request->power_loss = false;
    request->sea_status = CHILD_LOCK_SEA_SAFE;
    request->driver_confirmed_again = false;
    request->apply_result = CHILD_LOCK_APPLY_SUCCESS;
}

/**
 * @brief Initialize a response with safe defaults.
 * @param response Response object to initialize. NULL is ignored.
 * @param current_state Current child lock state preserved by default.
 * @return None.
 */
void ChildLockSystem_InitResponse(ChildLockResponse *response,
                                  ChildLockState current_state)
{
    if (response == NULL)
    {
        return;
    }

    response->new_child_lock_state = current_state;
    response->target_child_lock_state = current_state;
    response->display_message = NULL;
    response->result_status = CHILD_LOCK_RESULT_INVALID_REQUEST;
    response->fault_log_needed = false;
    response->command_sent = false;
    response->status_update_needed = false;
}

/**
 * @brief Check whether UC01 supports the requested door.
 * @param door_id Door identifier supplied by the caller.
 * @return true when RL or RR is requested, otherwise false.
 */
bool ChildLockSystem_IsDoorIdValid(ChildLockDoorId door_id)
{
    return (door_id == CHILD_LOCK_DOOR_RL) || (door_id == CHILD_LOCK_DOOR_RR);
}

/**
 * @brief Toggle the current child lock state.
 * @param current_state Current state before BCM processing.
 * @return The toggled target state.
 */
ChildLockState ChildLockSystem_ToggleState(ChildLockState current_state)
{
    return (current_state == CHILD_LOCK_STATE_OFF) ?
        CHILD_LOCK_STATE_ON : CHILD_LOCK_STATE_OFF;
}

/**
 * @brief Execute the UC01 child lock toggle decision logic.
 * @param request Input request data from BCM.
 * @param response Output result data for BCM/HMI handling.
 * @return High-level use case result status.
 */
ChildLockResultStatus ChildLockSystem_ProcessRequest(
    const ChildLockRequest *request,
    ChildLockResponse *response)
{
    if ((request == NULL) || (response == NULL))
    {
        return CHILD_LOCK_RESULT_INVALID_REQUEST;
    }

    ChildLockSystem_InitResponse(response, request->current_child_lock_state);

    if (!ChildLockSystem_IsDoorIdValid(request->door_id))
    {
        response->display_message = CHILD_LOCK_MSG_INVALID_DOOR;
        return response->result_status;
    }

    if (!request->acc_on)
    {
        response->display_message = CHILD_LOCK_MSG_ACC_OFF;
        response->result_status = CHILD_LOCK_RESULT_BLOCKED_ACC_OFF;
        return response->result_status;
    }

    if (request->speed_kph > 0U)
    {
        response->display_message = CHILD_LOCK_MSG_SPEED;
        response->result_status = CHILD_LOCK_RESULT_BLOCKED_SPEED;
        return response->result_status;
    }

    if (request->crash_override)
    {
        response->display_message = CHILD_LOCK_MSG_CRASH;
        response->result_status = CHILD_LOCK_RESULT_BLOCKED_CRASH;
        return response->result_status;
    }

    if (request->power_loss)
    {
        response->display_message = CHILD_LOCK_MSG_POWER;
        response->result_status = CHILD_LOCK_RESULT_BLOCKED_POWER;
        return response->result_status;
    }

    response->target_child_lock_state = ChildLockSystem_ToggleState(
        request->current_child_lock_state);

    if ((request->sea_status == CHILD_LOCK_SEA_RISK) &&
        (response->target_child_lock_state == CHILD_LOCK_STATE_OFF) &&
        (!request->driver_confirmed_again))
    {
        response->display_message = CHILD_LOCK_MSG_RECONFIRM;
        response->result_status = CHILD_LOCK_RESULT_CONFIRMATION_REQUIRED;
        return response->result_status;
    }

    response->command_sent = true;

    if (request->apply_result == CHILD_LOCK_APPLY_SUCCESS)
    {
        response->new_child_lock_state = response->target_child_lock_state;
        response->result_status = CHILD_LOCK_RESULT_OK;
        response->status_update_needed = true;
        return response->result_status;
    }

    if (request->apply_result == CHILD_LOCK_APPLY_FAIL)
    {
        response->display_message = CHILD_LOCK_MSG_APPLY_FAIL;
        response->result_status = CHILD_LOCK_RESULT_APPLY_FAILED;
        return response->result_status;
    }

    response->display_message = CHILD_LOCK_MSG_APPLY_TIMEOUT;
    response->result_status = CHILD_LOCK_RESULT_APPLY_TIMEOUT;
    response->fault_log_needed = true;
    return response->result_status;
}
