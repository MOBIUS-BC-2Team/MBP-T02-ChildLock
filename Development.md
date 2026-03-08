# Development.md
MBP-T02-ChildLock 개발 환경 정의서 (Version Pinning / GitHub Actions 기준)

본 문서는 **로컬 개발 환경과 GitHub Actions(CI) 환경을 동일하게 맞추기 위한** 개발 환경 정의서입니다.
프로젝트 목표/범위/사용도구는 README에 정리되어 있습니다. 설치/실행”을 고정합니다. :contentReference[oaicite:2]{index=2}

---

## 1) 기준 CI 환경 (GitHub Actions)
- Runner: `ubuntu-22.04`

> Runner 이미지에는 GNU C/C++ 11.4.0(및 다른 버전들)이 포함되어 있어, GCC 11.4 기반 표준화가 가능합니다. :contentReference[oaicite:3]{index=3}

---

## 2) Toolchain 버전 스펙
아래 버전을 **프로젝트 표준으로 고정(Pinning)** 합니다.

| Category | Tool | Version | Pinning 방식 |
|---|---|---:|---|
| Compiler | GCC | 11.4 | `ubuntu-22.04` + `gcc-11/g++-11` 고정 :contentReference[oaicite:4]{index=4} |
| Language | C Standard | C11 | `-std=c11` 고정 |
| Unit Test | GoogleTest | 1.12.1 | CMake FetchContent로 `v1.12.1` 고정 :contentReference[oaicite:5]{index=5} |
| Coverage | gcov | GCC 11 bundle | GCC 11과 함께 제공(gcov 버전 통일) |
| Static Analysis | cppcheck | 2.10 | 릴리즈(2.10) 고정 설치/빌드 :contentReference[oaicite:6]{index=6} |
| (Optional) LOC | cloc | 1.96 | release 1.96 번들 고정 :contentReference[oaicite:7]{index=7} |
| (Optional) Complexity | lizard | 1.17.10 | pip 버전 고정 :contentReference[oaicite:8]{index=8} |

### 2.1 GoogleTest(C++ 표준) 안내
- google/googletest 릴리즈 노트 기준으로 **v1.12.1은 C++11 지원의 마지막 릴리즈**로 안내되어 있습니다. :contentReference[oaicite:9]{index=9}  
- 본 프로젝트에서는 테스트 코드를 C++로 작성하며, **C 제품 코드를 링크**합니다.
- 권장:
  - 제품 코드: C11
  - 테스트 코드: C++14(또는 C++11)  
  *(향후 gtest 업그레이드(C++14+) 대비를 위해 C++14 권장)*

---

## 3) 레포 구조
현재 `doc/uml_diagram/`에 UML 산출물이 관리되고 있습니다. :contentReference[oaicite:10]{index=10}  
코드/테스트/CI는 아래 구조로 확장
├─ src/ # 제품 코드(C)
├─ include/ # 헤더
├─ tests/ # 단위 테스트(C++/GoogleTest)
├─ doc/uml_diagram/ # UML 산출물
├─ tools/ # cppcheck/cloc 고정 설치 파일 또는 스크립트
├─ .github/workflows/ # GitHub Actions(CI)
├─ Development.md #
└─ GITHUB.md # 레포 운영 정책

## 4) 로컬 개발 환경 구축 (Ubuntu 22.04 / WSL2 Ubuntu 22.04)
### 4.1 필수 패키지
```bash
sudo apt-get update
sudo apt-get install -y \
  gcc-11 g++-11 \
  cmake make git \
  python3 python3-pip

gcc-11 --version
g++-11 --version
gcov-11 --version
cmake --version
python3 --version