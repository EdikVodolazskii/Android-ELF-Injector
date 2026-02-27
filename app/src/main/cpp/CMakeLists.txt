cmake_minimum_required(VERSION 3.22.1)
project("agentexercise")

add_library(
        dynamicloader
        SHARED
        native-lib.cpp)

target_link_libraries(
        dynamicloader
        log)

add_library(
        hello
        SHARED
        hello.cpp)

target_link_libraries(
        hello
        log)