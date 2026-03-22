# UC-01 Child Lock ON/OFF Functional Test Cases

## Scope and Basis
- Use Case: `UC-01 Set Electronic Child Lock On/Off`
- Test basis: [`include/UC_1_child_lock_system_on_off.h`](../include/UC_1_child_lock_system_on_off.h), [`src/UC_1_child_lock_system_on_off.c`](../src/UC_1_child_lock_system_on_off.c)
- Key decision points:
  - 입력 유효성(도어 ID, ACC, 속도, 사고 플래그, 전원 플래그)
  - 토글 목표 상태 계산(ON/OFF)
  - SEA 위험 + 해제 목표 시 재확인 필요
  - 도어 ECU 적용 결과(SUCCESS/FAIL/TIMEOUT)

| Test Case ID | Test Object | Precondition | Input Data | Expected Output | Test Technique | Remarks |
|---|---|---|---|---|---|---|
| TC-UC01-001 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `door_id=CHILD_LOCK_DOOR_RL`, `current_state=OFF`, `acc_on=true`, `speed_kph=0`, `crash_override=false`, `power_loss=false`, `sea_status=SAFE`, `driver_confirmed_again=false`, `apply_result=SUCCESS` | `result_status=CHILD_LOCK_RESULT_OK`, `new_state=ON`, `command_sent=true`, `status_update_needed=true` | Equivalence Partitioning | 정상 동작 대표 케이스(잠금 OFF->ON) |
| TC-UC01-002 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `door_id=CHILD_LOCK_DOOR_INVALID` (기타 입력은 정상) | `result_status=CHILD_LOCK_RESULT_INVALID_REQUEST`, `command_sent=false`, `display_message=\"지원하지 않는 도어입니다.\"` | Equivalence Partitioning | 비유효 도어 ID 입력 차단 확인 |
| TC-UC01-003 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `acc_on=false` (기타 입력은 정상) | `result_status=CHILD_LOCK_RESULT_BLOCKED_ACC_OFF`, `command_sent=false`, `display_message=\"ACC OFF: 설정 변경 불가\"` | Equivalence Partitioning | ACC OFF 상태에서 설정 변경 금지 확인 |
| TC-UC01-004 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `speed_kph=0` (기타 입력은 정상) | `result_status=CHILD_LOCK_RESULT_OK` | Boundary Value Analysis | 속도 경계값 하한(정차=0)에서 통과 확인 |
| TC-UC01-005 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `speed_kph=1` (기타 입력은 정상) | `result_status=CHILD_LOCK_RESULT_BLOCKED_SPEED`, `command_sent=false`, `display_message=\"정차 후 변경 가능\"` | Boundary Value Analysis | 속도 경계값 바로 위(1)에서 차단 확인 |
| TC-UC01-006 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `crash_override=true` (기타 입력은 정상) | `result_status=CHILD_LOCK_RESULT_BLOCKED_CRASH`, `command_sent=false` | Error Guessing | FMEA 예외: 사고 감지 시 설정 변경 차단 |
| TC-UC01-007 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `power_loss=true` (기타 입력은 정상) | `result_status=CHILD_LOCK_RESULT_BLOCKED_POWER`, `command_sent=false` | Error Guessing | FMEA 예외: 전원 불안정 시 설정 변경 차단 |
| TC-UC01-008 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `current_state=ON`, `sea_status=RISK`, `driver_confirmed_again=false`, `apply_result=SUCCESS` (기타 입력 정상) | `result_status=CHILD_LOCK_RESULT_CONFIRMATION_REQUIRED`, `new_state=ON`, `target_state=OFF`, `command_sent=false` | Equivalence Partitioning | 위험 상황에서 해제 요청 시 재확인 요구 확인 |
| TC-UC01-009 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `current_state=ON`, `sea_status=RISK`, `driver_confirmed_again=true`, `apply_result=SUCCESS` (기타 입력 정상) | `result_status=CHILD_LOCK_RESULT_OK`, `new_state=OFF`, `command_sent=true` | Equivalence Partitioning | 재확인 후 해제 허용 경로 확인 |
| TC-UC01-010 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `apply_result=CHILD_LOCK_APPLY_FAIL` (기타 입력 정상) | `result_status=CHILD_LOCK_RESULT_APPLY_FAILED`, `new_state` 유지, `display_message=\"설정 실패 (도어 장치 오류)\"` | Error Guessing | 도어 ECU 장치 오류 응답 처리 확인 |
| TC-UC01-011 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `apply_result=CHILD_LOCK_APPLY_TIMEOUT` (기타 입력 정상) | `result_status=CHILD_LOCK_RESULT_APPLY_TIMEOUT`, `fault_log_needed=true`, `display_message=\"도어 응답 없음 설정 실패\"` | Error Guessing | 타임아웃 시 Fault Log 플래그 설정 확인 |
| TC-UC01-012 | `ChildLockSystem_ProcessRequest()` | 시스템 초기화 완료 | `speed_kph=65535` (기타 입력 정상) | `result_status=CHILD_LOCK_RESULT_BLOCKED_SPEED`, `command_sent=false` | Boundary Value Analysis | [AI Suggestion] `uint16_t` 상한 입력에서도 안전 차단 유지되는지 극단값 검증 |

