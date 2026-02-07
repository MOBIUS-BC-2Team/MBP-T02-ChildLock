# 🚗 ISO 26262 기반 SW 개발 시뮬레이션 프로젝트

### Electronic Child Lock System (전자식 차일드 락 시스템)

---

## 📌 프로젝트 개요

본 프로젝트는 **자동차 기능 안전 국제 표준 ISO 26262** 및
**A-SPICE 기반 소프트웨어 개발 프로세스**를 실습하기 위한
**SW 개발 시뮬레이션 PBL(Project-Based Learning)** 프로젝트입니다.

실습 대상 시스템으로는
**전자식 차일드 락 시스템(Electronic Child Lock System)** 을 선정하여,

* 사용자 요구사항 분석
* 기능 안전 분석 (FMEA / HAZOP)
* UML 기반 설계
* GitHub 기반 협업 개발
* 테스트 및 정적 분석
* 요구사항 추적성 관리

까지 **실제 자동차 SW 개발 흐름을 축소 재현**하는 것을 목표로 합니다.

---

## 🎯 프로젝트 목표

* ISO 26262 Part 3, Part 6 기반 **기능 안전 중심 SW 개발 경험**
* 요구사항 → 설계 → 구현 → 테스트 간 **V-모델 개발 흐름 이해**
* GitHub 기반 협업 환경(Code / Issue / Wiki / Action / PR) 실습
* 기능 안전 관점의 **SW 품질 확보 및 산출물 관리**
* 팀 프로젝트 결과물을 **취업용 포트폴리오로 활용 가능**하도록 정리

---

## 🚙 실습 대상 시스템: 전자식 차일드 락 시스템

### 🔹 시스템 개요

전자식 차일드 락 시스템은
뒷좌석 탑승자(어린이 등)의 **돌발적인 문 개방으로 인한 사고를 방지**하기 위해,
운전석에서 **뒷좌석 도어 잠금/해제 상태를 전자적으로 제어**하는 시스템입니다.

### 🔹 핵심 기능

* 운전석 버튼을 통해 **뒷좌석 좌/우 도어 개폐 가능 여부 제어**
* ECU가 도어 래치 내부 액추에이터를 구동
* Inside Handle(내부 손잡이)의 물리적 연결을 차단 또는 허용
* 주행 안전 확보를 위한 **안전 상태(Fail-safe) 고려**

> ⚠️ 본 프로젝트에서는 **‘전자식 잠금/해제’ 기능을 기본 기능으로 설정**하며,
> 추가 시스템 기능 및 확장 요구사항은 실습 과정에서 점진적으로 정의합니다.

---

## 🧩 프로젝트 범위

본 프로젝트는 다음 개발 단계를 포함합니다.

1. **사용자 요구사항 분석**

   * 시장 조사 및 사용자 시나리오 도출
   * 기능 요구사항 / 비기능 요구사항 정의

2. **기능 안전 분석 (ISO 26262)**

   * Hazard Identification
   * FMEA / HAZOP 기반 위험 분석
   * Safety Goal 및 Functional Safety Requirement 도출

3. **SW 요구사항 및 설계**

   * UML 기반 Use Case / Sequence / State Diagram 작성
   * 상위 수준 SW 아키텍처 설계

4. **개발 환경 구축**

   * GitHub 기반 협업 환경
   * CI/CT 자동화 (GitHub Action)

5. **구현 및 테스트**

   * C/C++ 기반 기능 구현
   * Google Test 기반 단위 테스트
   * Cppcheck 정적 분석

6. **추적성 관리**

   * 요구사항 ↔ 설계 ↔ 코드 ↔ 테스트 간 Traceability 확보

---

## 🛠️ 개발 환경 및 사용 도구

| 구분    | 도구                               |
| ----- | -------------------------------- |
| 형상관리  | GitHub                           |
| 협업    | GitHub Issue, Wiki, Pull Request |
| CI/CT | GitHub Action                    |
| 컴파일러  | GCC                              |
| 테스트   | Google Test                      |
| 정적 분석 | Cppcheck                         |
| 설계    | UML, Draw.io                     |

---

## 👥 팀 구성

| 이름  | 역할 |
| --- | -- |
| 윤영태 | 팀장 |
| 김동찬 | 팀원 |
| 박소희 | 팀원 |
| 이여규 | 팀원 |
| 한지윤 | 팀원 |

> 🔹 역할은 프로젝트 진행 과정에서 단계별로 유연하게 분담합니다.

---

## 📂 Repository 구성 (예정)

```
📦 Electronic-Child-Lock-System
 ┣ 📂 docs            # 프로젝트 문서 (설계, 분석)
 ┣ 📂 wiki            # GitHub Wiki 기반 산출물
 ┣ 📂 src             # 소프트웨어 소스 코드
 ┣ 📂 test            # 단위 테스트 코드
 ┣ 📂 ci              # CI 설정 파일
 ┣ 📜 README.md
```

---

## 📖 Wiki 안내

본 프로젝트의 주요 산출물은 **GitHub Wiki**를 통해 관리합니다.

* System Benchmark & Analysis
* User Requirement Specification
* Functional Safety Analysis (FMEA / HAZOP)
* UML 설계 산출물
* Traceability Matrix

📌 **Wiki는 프로젝트 진행 단계별로 지속적으로 업데이트됩니다.**

---

## 🧠 기대 효과

* 자동차 SW 개발 프로세스에 대한 **실무적 이해**
* 기능 안전 기반 사고 예방 설계 경험
* 협업 중심의 개발 문화 체험
* 개인 포트폴리오로 활용 가능한 **정형화된 프로젝트 산출물 확보**

---

## 📌 참고 표준

* ISO 26262: Road vehicles — Functional safety
* Automotive SPICE (A-SPICE)
