# MyInterObjComBase - Build, Run, and Test Guide

This project is configured with CMake for C11/C++17, GoogleTest auto-discovery from Test/*.cxx, and sanitizer build profiles.

## 1. Prerequisites

- CMake 3.16+
- A C/C++ compiler (clang or gcc)
- GoogleTest (optional if using FetchContent fallback)

## 2. Standard Build

```bash
cmake -S . -B build -DCONFIG_BUILD_UNIT_TESTING=ON
cmake --build build -j
```

## 3. Run Unit Tests

```bash
ctest --test-dir build --output-on-failure
```

You can also run each executable directly from build/Test/.

## 4. Sanitizer Profiles

Supported CMAKE_BUILD_TYPE values:

- DiagASAN
- DiagTSAN
- DiagUBSAN
- DiagMSAN
- DiagLSAN

Example:

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=DiagASAN -DCONFIG_BUILD_UNIT_TESTING=ON
cmake --build build-asan -j
ctest --test-dir build-asan --output-on-failure
```

## 5. Notes

- Each Test/*.cxx is built as an independent gtest executable.
- The current test files are CaTDD skeletons; they may run with zero concrete assertions until implementation starts.
- If Source/*.c is absent, the project library is configured as an interface target to keep test scaffolding operational.
