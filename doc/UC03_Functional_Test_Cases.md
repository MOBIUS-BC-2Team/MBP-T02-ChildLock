# UC-03 Safe Exit Assist Functional Test Cases

## Scope and Basis
- Use Case: `UC-03 Safe Exit Assist`
- Test basis: [`include/UC_3_safe_exit_assist.h`](../include/UC_3_safe_exit_assist.h), [`src/UC_3_safe_exit_assist.c`](../src/UC_3_safe_exit_assist.c)
- Decision points used for derivation:
  - `unlock_request` gate
  - `radar_status` fail-safe handling (`RADAR_TIMEOUT`, `RADAR_ERROR`)
  - `radar_object_speed` threshold at `6.0 km/h`

| Test Case ID | Test Object | Precondition | Input Data | Expected Output | Test Technique | Remarks |
|---|---|---|---|---|---|---|
| TC-UC03-001 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=false`, `radar_object_speed=0.0`, `radar_status=RADAR_NORMAL` | `door_cmd=MAINTAIN_LOCK`, `hmi_cmd=WARNING_OFF` | Equivalence Partitioning | 해제 요청이 없으면 기본 안전 상태를 유지해야 함 |
| TC-UC03-002 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=true`, `radar_object_speed=3.5`, `radar_status=RADAR_NORMAL` | `door_cmd=RELEASE_LOCK`, `hmi_cmd=WARNING_OFF` | Equivalence Partitioning | 안전 속도 구간(`6.0` 미만) 대표값 검증 |
| TC-UC03-003 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=true`, `radar_object_speed=6.5`, `radar_status=RADAR_NORMAL` | `door_cmd=MAINTAIN_LOCK`, `hmi_cmd=WARNING_ON` | Equivalence Partitioning | 위험 속도 구간(`6.0` 이상) 대표값 검증 |
| TC-UC03-004 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=true`, `radar_object_speed=5.9`, `radar_status=RADAR_NORMAL` | `door_cmd=RELEASE_LOCK`, `hmi_cmd=WARNING_OFF` | Boundary Value Analysis | 임계값 바로 아래 값 경계 검증 |
| TC-UC03-005 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=true`, `radar_object_speed=6.0`, `radar_status=RADAR_NORMAL` | `door_cmd=MAINTAIN_LOCK`, `hmi_cmd=WARNING_ON` | Boundary Value Analysis | 임계값 정확히 일치 시 분기(`>= 6.0`) 검증 |
| TC-UC03-006 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=true`, `radar_object_speed=6.1`, `radar_status=RADAR_NORMAL` | `door_cmd=MAINTAIN_LOCK`, `hmi_cmd=WARNING_ON` | Boundary Value Analysis | 임계값 바로 위 값 경계 검증 |
| TC-UC03-007 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=true`, `radar_object_speed=0.0`, `radar_status=RADAR_TIMEOUT` | `door_cmd=MAINTAIN_LOCK`, `hmi_cmd=ERROR_POPUP` | Error Guessing | FMEA 예외: 센서 통신 타임아웃 시 Fail-Safe 강제 |
| TC-UC03-008 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=true`, `radar_object_speed=10.0`, `radar_status=RADAR_ERROR` | `door_cmd=MAINTAIN_LOCK`, `hmi_cmd=ERROR_POPUP` | Error Guessing | FMEA 예외: 센서 오류 시 Fail-Safe 강제 |
| TC-UC03-009 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=false`, `radar_object_speed=0.0`, `radar_status=RADAR_TIMEOUT` | `door_cmd=MAINTAIN_LOCK`, `hmi_cmd=WARNING_OFF` | Equivalence Partitioning | 해제 요청 없음 분기가 센서 오류 분기보다 우선되는지 확인 |
| TC-UC03-010 | `Execute_Safe_Exit_Assist()` | SEA 로직 초기화 완료 | `unlock_request=true`, `radar_object_speed=-1.0`, `radar_status=RADAR_NORMAL` | `door_cmd=RELEASE_LOCK`, `hmi_cmd=WARNING_OFF` | Error Guessing | [AI Suggestion] 물리적으로 비정상인 음수 속도 입력에 대한 방어 로직 부재 여부 확인 |
