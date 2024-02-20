cmake_minimum_required(VERSION 3.1)

project(sol)

add_library(sol INTERFACE)

target_include_directories(sol INTERFACE include "${CMAKE_CURRENT_SOURCE_DIR}/")
target_include_directories(sol INTERFACE include "${CMAKE_CURRENT_SOURCE_DIR}/../lua/lua")
