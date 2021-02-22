# Developer Guide

## Table of Contents
* [Naming conventions](#naming-conventions)
* [GTest on Windows](#gtest-on-windows)

## Naming conventions

### Code

- Class names: `PascalCase`
- Class attributes: `snake_case`
- Local variables: `snake_case`
- Function parameters: `snake_case`
- Functions/Methods: `PascalCase`
- Namespaces: `snake_case`
- Typedefs: `snake_case_t`
- Enums: `PascalCase`
- Defines: `ALL_CAPS`

### Files

- Class files: `PascalCase.h`, `PascalCase.cpp`
- None class files: `snake_case.cpp`
- Directories: `snake_case`

### Git

#### Commits

- Start with a capital letter
- Present tense
- No dot at the end of the title

#### Branches

- `snake_case`

## GTest on Windows

1. Download a Google Test release of your choice from the official [GitHub Repository](https://github.com/google/googletest/).
2. Download the libraries [`libgtest.a`](https://drive.google.com/file/d/1N6hmXb2Ix6fvMJa3-2O6oXuaDyDnkBu6/view?usp=sharing) and [`libgtest_main.a`](https://drive.google.com/file/d/1wEpDVtbNIeSl22TKAJIqRLenjd2q6uM3/view?usp=sharing). (**use the links provided at your own risk**)
3. Copy both of these files into lib of MinGW (ex.: `C:\MinGW\lib`).
4. Go to your extracted GTest folder, navigate to `googletest\include\gtest` and copy the whole folder to `MinGW\lib\gcc\mingw32\[version]\include`.

[Source](https://medium.com/swlh/google-test-installation-guide-for-c-in-windows-for-visual-studio-code-2b2e66352456)
