
---

## GITHUB.md

```md
# GITHUB.md
MBP-T02-ChildLock Repository Policy (GitHub 운영 규칙)

본 프로젝트는 ISO 26262/A-SPICE 기반 SW 개발 흐름을 축소 재현하는 PBL 프로젝트이며,
UML 설계 산출물, 요구사항, 테스트/정적분석, 추적성 등을 GitHub에서 관리합니다. :contentReference[oaicite:17]{index=17}

---

## 1) 기본 원칙
- **재현성 우선**: 개발/CI 환경은 `Development.md`의 버전 스펙을 기준으로 고정합니다.
- **모든 변경은 PR로**: main 브랜치 직접 push 금지(보호 브랜치 권장).
- **CI 통과 후 merge**: 빌드/테스트/커버리지/정적분석이 통과해야 합니다.
- **문서/산출물도 형상관리**: UML 산출물은 `doc/uml_diagram/`에 관리합니다. :contentReference[oaicite:18]{index=18}

---

## 2) 브랜치 전략 (권장)
- `main`: 안정 브랜치(제출/공유 기준)
- 작업 브랜치 네이밍
  - `feat/<topic>` 기능
  - `fix/<topic>` 버그
  - `docs/<topic>` 문서
  - `chore/<topic>` 설정/리팩토링
  - `test/<topic>` 테스트

---

## 3) 이슈/PR 규칙
### 3.1 이슈
- 모든 작업은 이슈를 생성하고 진행합니다.
- 이슈에는 최소 포함:
  - 목표/범위(Definition of Done)
  - 영향 파일/모듈(예상)
  - 관련 문서 링크(해당 시)

### 3.2 PR
- PR은 해당 이슈를 연결합니다. (예: `Closes #12`)
- PR 템플릿(권장)
  - 변경 요약
  - 로컬 검증 결과(명령/결과)
  - CI 결과
  - (AI 사용 시) AI 사용 내역

---

## 4) GitHub Actions(CI) 정책
### 4.1 실행 트리거(권장)
- `pull_request`: 전체 CI 실행
- `push` to `main`: 전체 CI 실행

### 4.2 CI 체크 항목(최소)
- Build: GCC 11.4 / C11
- Unit Test: GoogleTest 1.12.1
- Coverage: gcov(GCC11)
- Static Analysis: cppcheck 2.10
- (옵션) cloc 1.96, lizard 1.17.10 리포트 artifact 업로드

> Ubuntu 22.04 runner에는 GNU C++ 11.4.0이 포함되어 있어 GCC11 기반 CI 구성에 적합합니다. :contentReference[oaicite:19]{index=19}

---

## 5) AI 사용 정책 (필수)
AI 도구(예: ChatGPT/Copilot 등)는 **허용**하되, 아래 규칙을 반드시 준수합니다.

### 5.1 허용 범위
- CMake/CI 스크립트 초안
- 단위 테스트 케이스 아이디어/스켈레톤 생성
- 정적 분석/커버리지 경고 해석 보조
- 문서 초안 작성(Development.md, UML 설명 등)

### 5.2 금지/주의
- **비밀정보(토큰/키/계정/내부자료) 입력 금지**
- 라이선스/출처가 불명확한 코드를 그대로 복붙 금지
- AI 결과물도 “작성자 책임”으로 리뷰/검증 후 반영

### 5.3 PR에 AI 사용 공개(권장)
PR 본문에 다음 중 하나를 표기합니다.
- `AI-assisted: Yes` / `AI-assisted: No`
- Yes인 경우:
  - 사용 범위(예: “CI yaml 초안”, “gtest 테스트 스켈레톤”)
  - 사람이 검증한 항목(예: “ctest pass”, “cppcheck 0 warnings”)

---

## 6) 문서/산출물 관리
- UML 산출물: `doc/uml_diagram/`에서 관리 :contentReference[oaicite:20]{index=20}
- 개발 환경: `Development.md`
- 요구사항/안전 분석/추적성: GitHub Wiki :contentReference[oaicite:21]{index=21}

---

## 7) 커밋/리뷰(권장)
- 커밋 메시지 예시
  - `feat: ...`, `fix: ...`, `docs: ...`, `chore: ...`, `test: ...`
- 리뷰 체크리스트(권장)
  - 기능 요구사항/UC와 일치
  - 테스트 추가/수정 포함
  - 정적 분석 경고 증가 여부
  - 커버리지 변화 확인(가능 시)

---