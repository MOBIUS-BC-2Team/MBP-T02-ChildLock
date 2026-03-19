/**
 * @file UC_1_child_lock_system_on_off.h
 * @brief Public interface for UC01 electronic child lock toggle control.
 * @details
 * Implemented: 2026-03-17 13:15 KST
 * Model: GPT-5 Codex
 * Version: v1.0
 *
 * This header defines the input, output, and service interfaces used to
 * evaluate the "Set Electronic Child Lock On/Off" use case for rear doors.
 *
 * Input values from the UC01 flow:
 * - doorId
 * - currentChildLockState
 * - accOn
 * - speed
 * - crashOverride
 * - powerLoss
 * - seaStatus
 * - driverConfirmedAgain
 * - applyResult
 *
 * Output values from the UC01 flow:
 * - newChildLockState
 * - displayMessage
 * - resultStatus
 * - faultLogNeeded
 */

#ifndef UC_1_CHILD_LOCK_SYSTEM_ON_OFF_H
#define UC_1_CHILD_LOCK_SYSTEM_ON_OFF_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Supported rear door identifiers for child lock control.
 */
typedef enum
{
    CHILD_LOCK_DOOR_RL = 0, /**< Rear-left door. */
    CHILD_LOCK_DOOR_RR = 1, /**< Rear-right door. */
    CHILD_LOCK_DOOR_INVALID = 2 /**< Invalid door identifier for validation. */
} ChildLockDoorId;

/**
 * @brief Electronic child lock state.
 */
typedef enum
{
    CHILD_LOCK_STATE_OFF = 0, /**< Child lock is released. */
    CHILD_LOCK_STATE_ON = 1   /**< Child lock is engaged. */
} ChildLockState;

/**
 * @brief SEA safety evaluation status.
 */
typedef enum
{
    CHILD_LOCK_SEA_SAFE = 0, /**< No rear-seat access risk is detected. */
    CHILD_LOCK_SEA_RISK = 1  /**< Rear-seat access risk is detected. */
} ChildLockSeaStatus;

/**
 * @brief Result reported by the door ECU or actuator.
 */
typedef enum
{
    CHILD_LOCK_APPLY_SUCCESS = 0, /**< Command applied successfully. */
    CHILD_LOCK_APPLY_FAIL = 1,    /**< Command failed because of door device error. */
    CHILD_LOCK_APPLY_TIMEOUT = 2  /**< No response was received in time. */
} ChildLockApplyResult;

/**
 * @brief High-level use case result returned to the caller.
 */
typedef enum
{
    CHILD_LOCK_RESULT_OK = 0,                  /**< Toggle completed successfully. */
    CHILD_LOCK_RESULT_BLOCKED_ACC_OFF = 1,     /**< ACC is OFF, so change is blocked. */
    CHILD_LOCK_RESULT_BLOCKED_SPEED = 2,       /**< Vehicle is moving, so change is blocked. */
    CHILD_LOCK_RESULT_BLOCKED_CRASH = 3,       /**< Crash override blocks the change. */
    CHILD_LOCK_RESULT_BLOCKED_POWER = 4,       /**< Power instability blocks the change. */
    CHILD_LOCK_RESULT_CONFIRMATION_REQUIRED = 5, /**< Risky unlock requires reconfirmation. */
    CHILD_LOCK_RESULT_APPLY_FAILED = 6,        /**< Door ECU reported failure. */
    CHILD_LOCK_RESULT_APPLY_TIMEOUT = 7,       /**< Door ECU timed out. */
    CHILD_LOCK_RESULT_INVALID_REQUEST = 8      /**< Invalid caller input was supplied. */
} ChildLockResultStatus;

/**
 * @brief UC01 request data for a single child lock toggle operation.
 * @param door_id Target rear door identifier.
 * @param current_child_lock_state Current door child lock state before toggle.
 * @param acc_on ACC state expected to be ON for a valid request.
 * @param speed_kph Current vehicle speed in kph.
 * @param crash_override Crash detection flag from safety logic.
 * @param power_loss Power instability flag.
 * @param sea_status SEA result used to guard risky unlock requests.
 * @param driver_confirmed_again Reconfirmation flag for risky unlock.
 * @param apply_result Door ECU execution result returned after command dispatch.
 */
typedef struct
{
    ChildLockDoorId door_id;
    ChildLockState current_child_lock_state;
    bool acc_on;
    uint16_t speed_kph;
    bool crash_override;
    bool power_loss;
    ChildLockSeaStatus sea_status;
    bool driver_confirmed_again;
    ChildLockApplyResult apply_result;
} ChildLockRequest;

/**
 * @brief UC01 response data produced after evaluating a toggle request.
 * @param new_child_lock_state Final child lock state after processing.
 * @param target_child_lock_state Requested target state after toggle decision.
 * @param display_message HMI message pointer, or NULL when not needed.
 * @param result_status High-level result of request processing.
 * @param fault_log_needed Flag that requests fault log creation.
 * @param command_sent Flag that reports whether BCM sent a door command.
 * @param status_update_needed Flag that reports whether HMI state refresh is needed.
 */
typedef struct
{
    ChildLockState new_child_lock_state;
    ChildLockState target_child_lock_state;
    const char *display_message;
    ChildLockResultStatus result_status;
    bool fault_log_needed;
    bool command_sent;
    bool status_update_needed;
} ChildLockResponse;

/**
 * @brief Initialize a request with safe default values.
 * @param request Request object to initialize. NULL is ignored.
 * @return None.
 */
void ChildLockSystem_InitRequest(ChildLockRequest *request);

/**
 * @brief Initialize a response with the current child lock state.
 * @param response Response object to initialize. NULL is ignored.
 * @param current_state Current child lock state retained as default output.
 * @return None.
 */
void ChildLockSystem_InitResponse(ChildLockResponse *response,
                                  ChildLockState current_state);

/**
 * @brief Check whether a door identifier is supported by UC01.
 * @param door_id Rear door identifier to validate.
 * @return true when the door is RL or RR, otherwise false.
 */
bool ChildLockSystem_IsDoorIdValid(ChildLockDoorId door_id);

/**
 * @brief Toggle a child lock state.
 * @param current_state Current child lock state.
 * @return CHILD_LOCK_STATE_ON when input is OFF, otherwise OFF.
 */
ChildLockState ChildLockSystem_ToggleState(ChildLockState current_state);

/**
 * @brief Execute the UC01 child lock toggle flow.
 * @param request Input data representing the BCM decision context.
 * @param response Output data filled with new state and HMI result.
 * @return Use case result status for the processed request.
 */
ChildLockResultStatus ChildLockSystem_ProcessRequest(
    const ChildLockRequest *request,
    ChildLockResponse *response);

#ifdef __cplusplus
}
#endif

#endif /* UC_1_CHILD_LOCK_SYSTEM_ON_OFF_H */
