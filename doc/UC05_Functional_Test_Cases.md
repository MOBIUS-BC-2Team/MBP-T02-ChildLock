# UC-05 Emergency Guidance Functional Test Cases

## Scope and Basis
- Use Case: `UC-05 Emergency Guidance`
- Test basis: [`include/UC_5_Emergency_Guidance.h`](../include/UC_5_Emergency_Guidance.h), [`src/UC_5_Emergency_Guidance.c`](../src/UC_5_Emergency_Guidance.c)
- Key decision points:
  - 뒷좌석 핸들 당김 요청 유무
  - 메인 전원 가용 여부
  - 백업 전원은 전자 래치 해제 결정에는 직접 반영되지 않음

| Test Case ID | Test Object | Precondition | Input Data | Expected Output | Test Technique | Remarks |
|---|---|---|---|---|---|---|
| TC-UC05-001 | `UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch()` | UC05 로직 초기화 완료 | `detect_rear_door_handle_pull=false`, `is_main_power_available=true`, `is_backup_power_available=true` | `false` | Equivalence Partitioning | 핸들 요청이 없으면 전원 상태와 무관하게 해제 금지 |
| TC-UC05-002 | `UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch()` | UC05 로직 초기화 완료 | `detect_rear_door_handle_pull=true`, `is_main_power_available=true`, `is_backup_power_available=false` | `true` | Equivalence Partitioning | 정상 경로: 요청 + 메인 전원 정상 시 해제 허용 |
| TC-UC05-003 | `UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch()` | UC05 로직 초기화 완료 | `detect_rear_door_handle_pull=true`, `is_main_power_available=false`, `is_backup_power_available=true` | `false` | Error Guessing | FMEA 예외: 메인 전원 상실 시 백업 전원만으로는 전자 해제 불가 |
| TC-UC05-004 | `UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch()` | UC05 로직 초기화 완료 | `detect_rear_door_handle_pull=true`, `is_main_power_available=false`, `is_backup_power_available=false` | `false` | Error Guessing | FMEA 예외: 전체 전원 상실 시 전자 해제 금지 |
| TC-UC05-005 | `UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch()` | UC05 로직 초기화 완료 | `detect_rear_door_handle_pull=true`, `is_main_power_available=true`, `is_backup_power_available=true` | `true` | Boundary Value Analysis | 백업 전원 입력 0/1 경계에서 결과 불변(메인 전원 우선) 확인 |
| TC-UC05-006 | `UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch()` | UC05 로직 초기화 완료 | `detect_rear_door_handle_pull=false`, `is_main_power_available=false`, `is_backup_power_available=false` | `false` | Boundary Value Analysis | 핸들 요청 경계(0/1)에서 0이면 즉시 해제 금지 확인 |
| TC-UC05-007 | `UC_5_Emergency_Guidance_EvaluateReleaseDoorLatch()` | UC05 로직 초기화 완료 | `detect_rear_door_handle_pull=true`, `is_main_power_available=true`에서 `is_backup_power_available`를 `false->true`로 변경 | 두 경우 모두 `true` 유지 | Error Guessing | [AI Suggestion] 구현상 `(void)is_backup_power_available` 처리이므로 회귀 방지용 불변성 테스트 추가 |

