function(log text)
    set(PADDING 10)
    string(REPEAT "=" ${PADDING} BORDER)
    message(STATUS "${BORDER} ${text} ${BORDER}")
endfunction()

log("Configuring Project")

cmake_minimum_required(VERSION 3.27)
project(stlviz LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

log("Fetching SFML")

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

log("Fetching GTest")

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(googletest)

log("Building Library stlviz")

# option(STLVIZ "Enable STL visualization" OFF)
# if(STLVIZ)
#   set(STLVIZ_ENABLED 1)
# else()
#   set(STLVIZ_ENABLED 0)
# endif()
set(STLVIZ_ENABLED 1)

file(GLOB_RECURSE CPP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(stlviz STATIC ${CPP_SOURCES})
target_compile_definitions(stlviz
    PRIVATE
        STLVIZ_ENABLED=$<BOOL:${STLVIZ_ENABLED}>
        STLVIZ_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)
target_include_directories(stlviz
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(stlviz PUBLIC SFML::Graphics SFML::Window SFML::System)

# ===== TESTS =====

log("Building Tests")

enable_testing()

add_executable(test_vector test/vstd/test_vector.cpp)
target_compile_definitions(test_vector PRIVATE TESTING STLVIZ_ENABLED=$<BOOL:${STLVIZ_ENABLED}>)
target_include_directories(test_vector
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(test_vector PRIVATE gtest_main stlviz)
add_test(NAME test_vector COMMAND test_vector)

# ===== INSTALL =====

log("Installing Library")

install(TARGETS stlviz
    EXPORT stlviz
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY assets/ DESTINATION assets)

install(EXPORT stlviz
    FILE stlviz.cmake
    NAMESPACE stlviz::
    DESTINATION lib/cmake/stlviz
)

# ===== BUILD DEBUG PROGRAM =====

log("Building Debug Program")

add_executable(main src/main.cpp)
target_compile_features(main PRIVATE cxx_std_20)
target_compile_definitions(main PRIVATE STLVIZ_ENABLED=1)
target_link_libraries(main PRIVATE stlviz)

# ===== BUILD USER STUDY PROGRAMS =====
file(GLOB_RECURSE USER_STUDY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/user_study/*.cpp")
message(STATUS "Found user study sources: ${USER_STUDY_SOURCES}")

foreach(src IN LISTS USER_STUDY_SOURCES)
    get_filename_component(exec_name ${src} NAME_WE)
    add_executable(${exec_name} ${src})
    target_compile_features(${exec_name} PRIVATE cxx_std_20)
    target_link_libraries(${exec_name} PRIVATE stlviz)
endforeach()