[안내] feat/test_from_wiki_doc 기반으로 재작성하고, feat/generate_testCase_file 케이스를 통합한 기능 테스트 케이스 문서입니다.

UC-01 차일드 락 시스템 제어

| Test Case ID | Test Object | Precondition | Input Data | Expected Output | Test Technique | Remarks |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| TC_UC01_001 | 정상 작동 차일드 락 켜기 | ACC ON, 차량 정차, 사고 전원상실 없음, 현재 OFF | toggleRequest = true<br>vehicleSpeed = 0<br>currentChildLockState = OFF<br>applyResult = Success | setChildLockCommand = ON<br>updatedChildLockState = ON<br>displayMessage = 상태 갱신 표시 | 동등 분할 | 모든 사전 조건을 만족할 때 정상적으로 차일드 락 설정 |
| TC_UC01_002 | 주행 중 조작 방지 | ACC ON, 주행 중 | toggleRequest = true<br>vehicleSpeed = 1 | keepCurrentState = true<br>displayMessage = 정차 후 변경 가능 | 경계값 분석 | 속도가 0을 초과하는 최소 경계값 1에서 즉시 거부 로직 작동 확인 |
| TC_UC01_003 | 충돌 감지 시 강제 해제 | ACC ON, 차량 정차, 충돌 발생 | toggleRequest = true<br>isCrashDetected = true | keepCurrentState = true<br>displayMessage = 사고 감지 설정 변경 불가 | 에러 추정 | 설계도 명시 사항 사고 감지 시 사용자 조작보다 안전 강제 해제를 최우선 |
| TC_UC01_004 | 전원 불안정 시 조작 방지 | ACC ON, 차량 정차, 전원 불안정 | toggleRequest = true<br>isCrashDetected = false<br>isPowerLoss = true | keepCurrentState = true<br>displayMessage = 전원 불안정 설정 변경 불가 | 에러 추정 | 설계도 명시 사항 전원 상실 시 로직 중단 |
| TC_UC01_005 | 하차 위험 시 취소 | 차량 정차, 하차 위험 감지, 현재 ON, 재확인 안 함 | seaStatus = Risk<br>targetChildLockState = OFF<br>driverReconfirm = false | keepCurrentState = true<br>displayMessage = 후방 접근 감지 재조작 시 해제 | 동등 분할 | 하차 안전 보조와 연계하여 1차 조작 시 경고만 띄우고 상태 유지 |
| TC_UC01_006 | 하차 위험 시 강행 | 차량 정차, 하차 위험 감지, 현재 ON, 강행 | seaStatus = Risk<br>targetChildLockState = OFF<br>driverReconfirm = true<br>applyResult = Success | setChildLockCommand = OFF<br>updatedChildLockState = OFF | 동등 분할 | 위험 상황 경고 후 운전자가 재조작 시 의도를 존중하여 해제 |
| TC_UC01_007 | 제어기 설정 실패 | 모든 사전 조건 통과, 제어 명령 전송 후 | applyResult = Fail | keepCurrentState = true<br>displayMessage = 설정 실패 도어 장치 오류 | 에러 추정 | 하드웨어 기계적 결함 발생 시 예외 처리 |
| TC_UC01_008 | 제어기 응답 없음 | 모든 사전 조건 통과, 제어 명령 전송 후 | applyResult = Timeout | keepCurrentState = true<br>faultStatus = 통신 장애 기록<br>displayMessage = 도어 응답 없음 설정 실패 | 에러 추정 | 통신 지연 및 단절 발생 시 예외 처리 |
| TC_UC01_009 | 비유효 도어 ID 차단 | ACC ON, 차량 정차, 기타 조건 정상 | doorId = INVALID<br>toggleRequest = true | keepCurrentState = true<br>displayMessage = 지원하지 않는 도어입니다. | 동등 분할 | 비유효 도어 입력 시 명령 전송 없이 요청 거절 |
| TC_UC01_010 | ACC OFF 상태 조작 방지 | ACC OFF, 차량 정차, 기타 조건 정상 | toggleRequest = true<br>accOn = false | keepCurrentState = true<br>displayMessage = ACC OFF: 설정 변경 불가 | 동등 분할 | 점화 OFF 상태에서 설정 변경 금지 |
| TC_UC01_011 | 정차 경계값 허용 | ACC ON, 차량 정차, 기타 조건 정상 | toggleRequest = true<br>vehicleSpeed = 0<br>currentChildLockState = OFF | setChildLockCommand = ON<br>updatedChildLockState = ON | 경계값 분석 | 속도 하한 경계(0)에서 정상 처리 확인 |
| TC_UC01_012 | 속도 극단값 차단 | ACC ON, 주행 중, 기타 조건 정상 | toggleRequest = true<br>vehicleSpeed = 65535 | keepCurrentState = true<br>displayMessage = 정차 후 변경 가능 | 경계값 분석 | [AI Suggestion] uint16 극단값 입력에서도 차단 유지 확인 |

UC-02 뒷좌석 도어 제어

| Test Case ID | Test Object | Precondition | Input Data | Expected Output | Test Technique | Remarks |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| TC_UC02_001 | 정상 개방 차일드락 끄기 | 차량 정차, 모든 시스템 정상, 차일드락 OFF | isHandlePulled = true<br>isSafetyValid = true<br>isLatchSensorError = false<br>isECUError = false<br>isPowerError = false<br>isChildLockActive = false | doorOpenStatus = 도어 개방 | 동등 분할 | 모든 안전 시스템 조건이 충족되고 차일드 락이 꺼져 있을 때 정상 개방 확인 |
| TC_UC02_002 | 개방 차단 차일드락 켜기 | 차량 정차, 모든 시스템 정상, 차일드락 ON | isHandlePulled = true<br>isSafetyValid = true<br>isLatchSensorError = false<br>isECUError = false<br>isPowerError = false<br>isChildLockActive = true | doorOpenStatus = 도어 차단 | 동등 분할 | 차일드 락 본연의 기능 활성화 시 내부 개방 시도 차단 |
| TC_UC02_003 | 안전 조건 미충족 차단 | 주행 중이거나 도어 잠금 상태 등 안전 조건 위배 | isHandlePulled = true<br>isSafetyValid = false | doorOpenStatus = 도어 차단 | 동등 분할 | 기본 안전 조건 미충족 시 가장 먼저 개방 차단 로직 작동 |
| TC_UC02_004 | 도어 래치 센서 고장 | 센서 단선 단락 등 물리적 오류 발생 | isHandlePulled = true<br>isSafetyValid = true<br>isLatchSensorError = true | Display = 도어 래치 센서 오류 도어 개방 불가<br>doorOpenStatus = 도어 차단 | 에러 추정 | 하드웨어 센서 에러 시 개방 차단 및 사용자 알림 확인 |
| TC_UC02_005 | 제어기 오류 | 통신 불량 또는 내부 연산 오류 발생 | isHandlePulled = true<br>isSafetyValid = true<br>isLatchSensorError = false<br>isECUError = true | Display = ECU 오류 도어 개방 불가<br>doorOpenStatus = 도어 차단 | 에러 추정 | 제어기 에러 시 개방 차단 및 사용자 알림 확인 |
| TC_UC02_006 | 차량 전원 불안정 | 배터리 전압 강하 등 전원 이상 감지 | isHandlePulled = true<br>isSafetyValid = true<br>isLatchSensorError = false<br>isECUError = false<br>isPowerError = true | Display = 차량 전원 불안정 도어 개방 불가<br>doorOpenStatus = 도어 차단 | 에러 추정 | 전원 부족으로 인한 오작동 방지를 위해 개방 차단 및 사용자 알림 확인 |
| TC_UC02_007 | 미조작 상태 대기 | 탑승자가 손잡이를 당기지 않은 상태 | isHandlePulled = false | 도어 개방 차단 로직 미실행 상태 유지 | 동등 분할 | 조작이 발생하지 않았을 때는 시스템이 불필요하게 동작하지 않음을 확인 |
| TC_UC02_008 | 손잡이 입력 경계(1) 통과 | 차량 정차, 기타 조건 정상, 차일드락 OFF | isHandlePulled = true<br>isSafetyValid = true<br>isLatchSensorError = false<br>isECUError = false<br>isPowerError = false<br>isChildLockActive = false | doorOpenStatus = 도어 개방 | 경계값 분석 | 손잡이 입력 경계값(0/1) 중 1에서 다음 판단 로직 진입 확인 |
| TC_UC02_009 | 복수 고장 동시 입력 차단 | 차량 정차, 복수 고장 발생 | isHandlePulled = true<br>isSafetyValid = false<br>isLatchSensorError = true<br>isECUError = true<br>isPowerError = true<br>isChildLockActive = true | doorOpenStatus = 도어 차단 | 에러 추정 | [AI Suggestion] 고장 중첩 입력에서도 일관된 차단 유지 확인 |

UC-03 하차 안전 보조

| Test Case ID | Test Object | Precondition | Input Data | Expected Output | Test Technique | Remarks |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| TC_UC03_001 | 하차 보조 기본 상태 | 차량 정차 상태, 센서 정상 작동 | unlock_request = false<br>radar_object_speed = 10.0<br>radar_status = NORMAL | door_cmd = MAINTAIN_LOCK<br>hmi_cmd = WARNING_OFF | 동등 분할 | 하차 요청이 없을 때는 위험 객체가 있어도 기본 잠금 상태 유지 및 경고 미발생 |
| TC_UC03_002 | 정상 하차 | 차량 정차 상태, 센서 정상 작동 | unlock_request = true<br>radar_object_speed = 3.5<br>radar_status = NORMAL | door_cmd = RELEASE_LOCK<br>hmi_cmd = WARNING_OFF | 동등 분할 | 안전한 속도일 때 정상적으로 잠금 해제 허용 |
| TC_UC03_003 | 위험 감지 | 차량 정차 상태, 센서 정상 작동 | unlock_request = true<br>radar_object_speed = 15.0<br>radar_status = NORMAL | door_cmd = MAINTAIN_LOCK<br>hmi_cmd = WARNING_ON | 동등 분할 | 위험 속도로 접근하는 차량 감지 시 잠금 유지 및 경고 발생 |
| TC_UC03_004 | 위험 속도 하한 경계 | 차량 정차 상태, 센서 정상 작동 | unlock_request = true<br>radar_object_speed = 5.9<br>radar_status = NORMAL | door_cmd = RELEASE_LOCK<br>hmi_cmd = WARNING_OFF | 경계값 분석 | 위험 기준 6.0의 바로 아래 경계값 안전 처리 |
| TC_UC03_005 | 위험 속도 기준 경계 | 차량 정차 상태, 센서 정상 작동 | unlock_request = true<br>radar_object_speed = 6.0<br>radar_status = NORMAL | door_cmd = MAINTAIN_LOCK<br>hmi_cmd = WARNING_ON | 경계값 분석 | 위험 기준 6.0과 정확히 일치하는 경계값 위험 처리 |
| TC_UC03_006 | 위험 속도 상한 경계 | 차량 정차 상태, 센서 정상 작동 | unlock_request = true<br>radar_object_speed = 6.1<br>radar_status = NORMAL | door_cmd = MAINTAIN_LOCK<br>hmi_cmd = WARNING_ON | 경계값 분석 | 위험 기준 6.0을 살짝 초과하는 경계값 위험 처리 |
| TC_UC03_007 | 센서 통신 단절 | 차량 정차 상태, 하차 시도 중 센서 응답 없음 | unlock_request = true<br>radar_object_speed = 0.0<br>radar_status = TIMEOUT | door_cmd = MAINTAIN_LOCK<br>hmi_cmd = ERROR_POPUP | 에러 추정 | 통신 지연 시 안전 로직 작동하여 강제 잠금 및 에러 팝업 |
| TC_UC03_008 | 센서 기계적 오류 | 차량 정차 상태, 센서 하드웨어 에러 발생 | unlock_request = true<br>radar_object_speed = 20.0<br>radar_status = ERROR | door_cmd = MAINTAIN_LOCK<br>hmi_cmd = ERROR_POPUP | 에러 추정 | 센서 고장 시 안전 로직 작동하여 강제 잠금 및 에러 팝업 |
| TC_UC03_009 | 비정상 속도값 입력 | 차량 정차 상태, 노이즈 발생 | unlock_request = true<br>radar_object_speed = -5.0<br>radar_status = NORMAL | door_cmd = MAINTAIN_LOCK<br>hmi_cmd = ERROR_POPUP | 에러 추정 | 음수 속도값 입력에 대한 예외 처리 |
| TC_UC03_010 | 해제 미요청 우선 분기 | 차량 정차 상태, 센서 TIMEOUT 발생 | unlock_request = false<br>radar_object_speed = 0.0<br>radar_status = TIMEOUT | door_cmd = MAINTAIN_LOCK<br>hmi_cmd = WARNING_OFF | 동등 분할 | 해제 요청 미입력 조건이 센서 오류 분기보다 우선되는지 확인 |
| TC_UC03_011 | 음수 속도 입력 구현 검증 | 차량 정차 상태, 센서 정상 작동 | unlock_request = true<br>radar_object_speed = -1.0<br>radar_status = NORMAL | door_cmd = RELEASE_LOCK<br>hmi_cmd = WARNING_OFF | 에러 추정 | [AI Suggestion] 현재 구현 동작 검증용 케이스(요구사항과 불일치 여부 점검) |

UC-05 전원 상실 시 비상 개방

| Test Case ID | Test Object | Precondition | Input Data | Expected Output | Test Technique | Remarks |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| TC_UC05_001 | 정상 전원 하차 | 차량 메인 배터리 전압 정상 | detectRearDoorHandlePull = true<br>isMainPowerAvailable = true | releaseDoorLatch 실행, 정상 작동 도어 열림 | 동등 분할 | 메인 전원이 살아있을 때 정상적인 전자식 개방 흐름 확인 |
| TC_UC05_002 | 예비 전원 작동 및 비상 안내 | 메인 배터리 이상, 예비 배터리 전압 정상 | detectRearDoorHandlePull = true<br>isMainPowerAvailable = false<br>isBackupPowerAvailable = true | 메인 전원 상실 확인됨, 예비 전원 공급, 비상 절차 안내 출력, 기계식 비상 개방 레버 조작 후 도어 열림 | 에러 추정 | 메인 전원 고장 시 예비 전원을 이용한 탑승자 비상 절차 안내 로직 점검 |
| TC_UC05_003 | 전체 전원 상실 및 최후 탈출 | 메인 배터리와 예비 배터리 모두 이상 | detectRearDoorHandlePull = true<br>isMainPowerAvailable = false<br>isBackupPowerAvailable = false | 메인 전원 상실 확인됨, 비상 절차 안내 불가, 기계식 비상 개방 레버 조작 후 도어 열림 | 에러 추정 | 최악의 상황인 전체 전원 상실 시나리오에서 기계적 탈출 보장 확인 |
| TC_UC05_004 | 손잡이 미조작 대기 상태 | 차량 정차, 모든 전원 상태 대기 | detectRearDoorHandlePull = false | 도어 개방 및 비상 절차 관련 하위 로직 진입 안 함 | 동등 분할 | 조작이 발생하지 않았을 때는 불필요한 개방 안내가 실행되지 않음을 점검 |
