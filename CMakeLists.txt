cmake_minimum_required(VERSION 3.15)
project(AutoRelay CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(AutoRelay
    src/main.cpp
    src/FileWatcher.cpp
    src/FileClassifier.cpp
    src/FileMover.cpp
    src/PathInput.cpp
    src/ProgressBar.cpp
)

target_compile_options(AutoRelay PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
)

# Older GCC/Clang (< GCC 9, < Clang 9) ship std::filesystem in a separate
# library that must be linked explicitly. Harmless no-op on newer toolchains
# where it isn't needed.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(AutoRelay PRIVATE stdc++fs)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(AutoRelay PRIVATE c++fs)
endif()