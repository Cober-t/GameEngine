cmake_minimum_required(VERSION 3.20)

project(Cober
    VERSION 1.0.0
    LANGUAGES CXX C
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option Build types
option(COBER_BUILD_EDITOR "Build editor" ON)
option(COBER_BUILD_GAME "Build sample game" ON)
option(COBER_ENABLE_ASAN "Enable AddressSanitizer" OFF)

# Configuration types
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release Dist)
endif()

if(MSVC)
    add_compile_options(/MP /wd4251 /wd4131 /wd4267)
endif()

# Global definitions
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

# Configuration-specific definitions
add_compile_definitions(
    _CRT_SECURE_NO_WARNINGS
    $<$<CONFIG:Debug>:COBER_DEBUG>
    $<$<CONFIG:Release>:COBER_RELEASE>
    $<$<CONFIG:Dist>:COBER_DIST>
)

# AddressSanitizer configuration
function(enable_address_sanitizer target)
    if(COBER_ENABLE_ASAN)
        if(MSVC)
            target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>:/fsanitize=address>)
            target_link_options(${target} PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL:NO>)
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(${target} PRIVATE
                $<$<CONFIG:Debug>:-fsanitize=address>
                $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
                $<$<CONFIG:Debug>:-g>
            )
            target_link_options(${target} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address>)
        endif()

        target_compile_definitions(${target} PRIVATE $<$<CONFIG:Debug>:ASAN_ENABLED>)
    endif()
endfunction()

# Include directories for libraries
set(SDL3_INCLUDE_DIR         ${CMAKE_SOURCE_DIR}/libraries/SDL/include)
set(IMGUI_INCLUDE_DIR        ${CMAKE_SOURCE_DIR}/libraries/imgui)
set(IMGUIZMO_INCLUDE_DIR     ${CMAKE_SOURCE_DIR}/libraries/ImGuizmo)
set(GLM_INCLUDE_DIR          ${CMAKE_SOURCE_DIR}/libraries/glm)
set(ENTT_INCLUDE_DIR         ${CMAKE_SOURCE_DIR}/libraries/entt/single_include)
set(JSON_INCLUDE_DIR         ${CMAKE_SOURCE_DIR}/libraries/json/single_include)
set(MSDF_INCLUDE_DIR         ${CMAKE_SOURCE_DIR}/libraries/msdf/msdf-atlas-gen)
set(MSDFGEN_INCLUDE_DIR      ${CMAKE_SOURCE_DIR}/libraries/msdf/msdfgen)
set(BOX2D_INCLUDE_DIR        ${CMAKE_SOURCE_DIR}/libraries/box2d/include)
set(SPDLOG_INCLUDE_DIR       ${CMAKE_SOURCE_DIR}/libraries/spdlog/include)
set(STB_IMAGE_INCLUDE_DIR    ${CMAKE_SOURCE_DIR}/libraries/stb_image/include)
set(STB_TRUETYPE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libraries/stb_truetype)
set(LUA_INCLUDE_DIR          ${CMAKE_SOURCE_DIR}/libraries/lua)
set(SOL_INCLUDE_DIR          ${CMAKE_SOURCE_DIR}/libraries/sol/include)

# Configure library options before adding subdirectories
# ==== SDL3= ============================================
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL3_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
# ==== BOX2D ============================================
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# ==== SPDLOG ===========================================
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
# ==== MSDF =============================================
set(MSDF_ATLAS_BUILD_STANDALONE OFF CACHE BOOL "" FORCE)
set(MSDFGEN_BUILD_STANDALONE OFF CACHE BOOL "" FORCE)
set(MSDFGEN_USE_VCPKG OFF CACHE BOOL "" FORCE)
# ==== GLM ==============================================
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

# Add subdirectories for dependencies
add_subdirectory(libraries)
add_subdirectory(Engine)

# Add subdirectories for core components
if(COBER_BUILD_EDITOR)
    add_subdirectory(Editor)
endif()

if(COBER_BUILD_GAME)
    add_subdirectory(Game)
endif()

if(COBER_BUILD_EDITOR)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)
elseif(COBER_BUILD_GAME)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)
endif()
