cmake_minimum_required(VERSION 3.1)

project(entt)

add_library(entt INTERFACE)

target_include_directories(entt INTERFACE include "${CMAKE_CURRENT_SOURCE_DIR}/")
