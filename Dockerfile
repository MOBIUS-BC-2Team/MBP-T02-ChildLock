FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive \
    TZ=Asia/Seoul \
    CC=gcc-11 \
    CXX=g++-11

# Project-standard toolchain based on Development.md
# - Ubuntu 22.04
# - GCC/G++ 11
# - C11 product code
# - GoogleTest 1.12.1 (recommended to pin in CMake with FetchContent)
# - gcov bundled with GCC 11
# - cppcheck 2.10
# - cloc 1.96
# - lizard 1.17.10
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        tzdata \
        ca-certificates \
        curl \
        wget \
        unzip \
        git \
        make \
        cmake \
        ninja-build \
        perl \
        python3 \
        python3-pip \
        gcc-11 \
        g++-11 \
        libpcre3-dev \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 110 \
    && python3 -m pip install --no-cache-dir lizard==1.17.10 \
    \
    # cppcheck 2.10 (pinned)
    && git clone --depth 1 --branch 2.10 https://github.com/danmar/cppcheck.git /tmp/cppcheck \
    && cmake -S /tmp/cppcheck -B /tmp/cppcheck/build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    && cmake --build /tmp/cppcheck/build \
    && cmake --install /tmp/cppcheck/build \
    && rm -rf /tmp/cppcheck \
    \
    # cloc 1.96 (pinned source script from official tag)
    && curl -fsSL https://raw.githubusercontent.com/AlDanial/cloc/v1.96/cloc -o /usr/local/bin/cloc \
    && chmod +x /usr/local/bin/cloc \
    \
    && gcc --version \
    && g++ --version \
    && gcov-11 --version \
    && cmake --version \
    && cppcheck --version \
    && cloc --version \
    && python3 -m lizard --version \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Recommended project workspace
WORKDIR /workspace

# Default shell; compose keeps the container alive separately.
CMD ["bash"]
