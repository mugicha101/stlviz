$ErrorActionPreference = "Stop"
cmake -B build_windows
cmake --build build_windows --config Release
ctest --verbose -C Release --test-dir build_windows
