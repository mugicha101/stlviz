# CMake SFML Project Template

This repository was forked from [CMake SFML Project Template](https://github.com/SFML/cmake-sfml-project.git), which provides basic support for building cross-platform SFML.

For more detailed build help, refer to the template's readme.

Note: Run all scripts from the root directory (`stlviz/`).

## Building

Note: Debug config causes SFML to error

### Linux

Run once to install necessary dependencies: `sh linux_setup.sh`

Run to build and test vstd library (outputs to `build_linux/`): `sh build.sh`

### Windows

No need to install dependencies.

Run to build and test vstd library (outputs to `build_windows/`): `./build.ps1`

### macOS

Prerequisites:
- Xcode Command Line Tools (provides `clang` and SDK): `xcode-select --install`
- CMake ≥ 3.27 (for example via Homebrew: `brew install cmake`)

From the project root (`stlviz/`), build and run tests:

`cmake -S . -B build_macos -DCMAKE_BUILD_TYPE=Release`
`cmake --build build_macos --config Release`
`ctest --verbose -C Release --test-dir build_macos`

## Installing

After confirming the build works, build and install using the following scripts:

Linux: `sh install.sh`

Windows: `./install.ps1`

macOS: `cmake --install build_macos`

Example use in another program's CMakeLists.txt:

```
find_package(stlviz)
target_compile_definitions(app PRIVATE)
target_link_libraries(app PRIVATE vstd)
```

Run an example file: `./build_macOS/bin/main` 
