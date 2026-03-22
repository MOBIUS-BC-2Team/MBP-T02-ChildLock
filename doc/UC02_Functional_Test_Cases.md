# UC-02 Rear Passenger Door Control Functional Test Cases

## Scope and Basis
- Use Case: `UC-02 Control Rear Passenger Door`
- Test basis: [`include/UC_2_control_rear_passenger_door.h`](../include/UC_2_control_rear_passenger_door.h), [`src/UC_2_control_rear_passenger_door.c`](../src/UC_2_control_rear_passenger_door.c)
- Key decision points:
  - 실내 핸들 조작 여부
  - 안전 유효성
  - 센서/ECU/전원 오류 상태
  - 차일드락 활성 상태

| Test Case ID | Test Object | Precondition | Input Data | Expected Output | Test Technique | Remarks |
|---|---|---|---|---|---|---|
| TC-UC02-001 | `control_rear_passenger_door()` | 도어 제어 로직 초기화 완료 | `is_handle_pulled=true`, `is_safety_valid=true`, `is_latch_sensor_error=false`, `is_ecu_error=false`, `is_power_error=false`, `is_child_lock_active=false` | `DOOR_OPEN_ALLOWED` | Equivalence Partitioning | 정상 조건 조합에서 도어 개방 허용 확인 |
| TC-UC02-002 | `control_rear_passenger_door()` | 도어 제어 로직 초기화 완료 | `is_handle_pulled=false` (기타 입력 정상) | `DOOR_OPEN_BLOCKED` | Boundary Value Analysis | 핸들 입력 경계(0/1) 중 0에서 즉시 차단 확인 |
| TC-UC02-003 | `control_rear_passenger_door()` | 도어 제어 로직 초기화 완료 | `is_handle_pulled=true` (기타 입력 정상) | `DOOR_OPEN_ALLOWED` | Boundary Value Analysis | 핸들 입력 경계(0/1) 중 1에서 다음 조건으로 진행 확인 |
| TC-UC02-004 | `control_rear_passenger_door()` | 도어 제어 로직 초기화 완료 | `is_safety_valid=false` (기타 입력 정상) | `DOOR_OPEN_BLOCKED` | Equivalence Partitioning | 안전 조건 비유효 클래스 차단 확인 |
| TC-UC02-005 | `control_rear_passenger_door()` | 도어 제어 로직 초기화 완료 | `is_latch_sensor_error=true` (기타 입력 정상) | `DOOR_OPEN_BLOCKED` | Error Guessing | FMEA 예외: 래치 센서 오류 시 Fail-Safe 차단 |
| TC-UC02-006 | `control_rear_passenger_door()` | 도어 제어 로직 초기화 완료 | `is_ecu_error=true` (기타 입력 정상) | `DOOR_OPEN_BLOCKED` | Error Guessing | FMEA 예외: ECU 오류 시 Fail-Safe 차단 |
| TC-UC02-007 | `control_rear_passenger_door()` | 도어 제어 로직 초기화 완료 | `is_power_error=true` (기타 입력 정상) | `DOOR_OPEN_BLOCKED` | Error Guessing | FMEA 예외: 전원 이상 시 Fail-Safe 차단 |
| TC-UC02-008 | `control_rear_passenger_door()` | 도어 제어 로직 초기화 완료 | `is_child_lock_active=true` (기타 입력 정상) | `DOOR_OPEN_BLOCKED` | Equivalence Partitioning | 차일드락 활성 상태에서 도어 개방 차단 확인 |
| TC-UC02-009 | `control_rear_passenger_door()` | 도어 제어 로직 초기화 완료 | `is_safety_valid=false`, `is_latch_sensor_error=true`, `is_ecu_error=true`, `is_power_error=true`, `is_child_lock_active=true`, `is_handle_pulled=true` | `DOOR_OPEN_BLOCKED` | Error Guessing | [AI Suggestion] 복수 고장 동시 입력 시에도 일관된 차단 유지 확인(현장 고장 중첩 대비) |

