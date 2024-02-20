cmake_minimum_required(VERSION 3.7)

project(lua)

add_library(
    lua STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lapi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lauxlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lauxlib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lbaselib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lcode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lcorolib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lctype.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lctype.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldblib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldebug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldebug.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ldump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lfunc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lfunc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lgc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lgc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/linit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/liolib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ljumptab.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/llex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/llex.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/llimits.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lmathlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lmem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/loadlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lopcodes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lopcodes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lopnames.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/loslib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lparser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lparser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lprefix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lstate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lstate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lstring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lstring.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lstrlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ltable.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ltable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ltablib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ltm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/ltm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lua.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lua.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lua.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/luac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/luaconf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lualib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lundump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lundump.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lutf8lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lvm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lvm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lzio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/lzio.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)