cmake_minimum_required(VERSION 3.22.1)
project("agentexercise")

set(LIEF_DOC OFF CACHE BOOL "Build LIEF docs" FORCE)
set(LIEF_PYTHON_API OFF CACHE BOOL "Build LIEF Python API" FORCE)
set(LIEF_EXAMPLES OFF CACHE BOOL "Build LIEF examples" FORCE)
set(LIEF_TESTS OFF CACHE BOOL "Build LIEF tests" FORCE)

include(FetchContent)
FetchContent_Declare(LIEF
        GIT_REPOSITORY "https://github.com/lief-project/LIEF.git"
        GIT_TAG 0.14.1
)

FetchContent_MakeAvailable(LIEF)

add_library(
        hello
        SHARED
        hello.cpp)
target_link_libraries(
        hello
        log)

add_library(
        dynamicloader
        SHARED
        native-lib.cpp)

target_link_libraries(
        dynamicloader
        LIEF::LIEF
        log)