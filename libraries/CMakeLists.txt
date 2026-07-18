# Libraries CMakeLists.txt
# This file adds existing library submodules using their own CMake configurations

# Add libraries that have their own CMakeLists.txt
# The options for these libraries are set in the root CMakeLists.txt

# SDL3 - Media library
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/SDL/CMakeLists.txt)
    message(STATUS "Adding SDL3 library")
    add_subdirectory(SDL)
    # Set folder for SDL targets
    get_property(SDL_TARGETS DIRECTORY SDL PROPERTY BUILDSYSTEM_TARGETS)
    foreach(target ${SDL_TARGETS})
        if(TARGET ${target})
            set_target_properties(${target} PROPERTIES FOLDER "Dependencies/SDL")
        endif()
    endforeach()
endif()

# Box2D - Physics library
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/box2d/CMakeLists.txt)
    message(STATUS "Adding Box2D library")
    add_subdirectory(box2d)
    if(TARGET box2d)
        set_target_properties(box2d PROPERTIES FOLDER "Dependencies")
    endif()
endif()

# spdlog - Logging library
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/CMakeLists.txt)
    message(STATUS "Adding spdlog library")
    add_subdirectory(spdlog)
    if(TARGET spdlog)
        set_target_properties(spdlog PROPERTIES FOLDER "Dependencies")
    endif()
endif()

# GLM - Mathematics library
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glm/CMakeLists.txt)
    message(STATUS "Adding GLM library")
    add_subdirectory(glm)
    if(TARGET glm)
        set_target_properties(glm PROPERTIES FOLDER "Dependencies")
    endif()
endif()

# MSDF - Multi-channel signed distance field
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/msdf/CMakeLists.txt)
    message(STATUS "Adding MSDF library")
    add_subdirectory(msdf)
    # Set folder for MSDF targets
    if(TARGET msdf-atlas-gen)
        set_target_properties(msdf-atlas-gen PROPERTIES FOLDER "Dependencies/MSDF")
    endif()
    if(TARGET msdfgen)
        set_target_properties(msdfgen PROPERTIES FOLDER "Dependencies/MSDF")
    endif()
    if(TARGET msdfgen-core)
        set_target_properties(msdfgen-core PROPERTIES FOLDER "Dependencies/MSDF")
    endif()
endif()

# SDL_shadercross - Shader cross-compilation
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/SDL_shadercross/CMakeLists.txt)
    message(STATUS "Adding SDL_shadercross library")
    set(SDL_SHADERCROSS_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_SHADERCROSS_TESTS OFF CACHE BOOL "" FORCE)
    set(SDLSHADERCROSS_VENDORED ON CACHE BOOL "" FORCE)
    set(SDLSHADERCROSS_DXC OFF CACHE BOOL "" FORCE)
    set(SDLSHADERCROSS_CLI OFF CACHE BOOL "" FORCE)
    add_subdirectory(SDL_shadercross)
    if(TARGET SDL3_shadercross-static)
        set_target_properties(SDL3_shadercross-static PROPERTIES FOLDER "Dependencies")
    endif()
endif()

# Libraries without CMakeLists.txt or requiring custom build logic
# These will be built with custom configurations

# ImGui - Immediate mode GUI
# ImGui - Immediate mode GUI
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
    message(STATUS "Adding ImGui library (custom build)")

    set(IMGUI_SOURCES
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
        imgui/backends/imgui_impl_sdl3.cpp
        imgui/backends/imgui_impl_sdlgpu3.cpp
    )

    add_library(ImGui STATIC ${IMGUI_SOURCES})

    target_include_directories(ImGui
        PUBLIC
            imgui
            imgui/backends
            ${CMAKE_CURRENT_SOURCE_DIR}/SDL/include
    )

    if(TARGET SDL3::SDL3-static)
        target_link_libraries(ImGui PUBLIC SDL3::SDL3-static)
    elseif(TARGET SDL3-static)
        target_link_libraries(ImGui PUBLIC SDL3-static)
    elseif(TARGET SDL3::SDL3)
        target_link_libraries(ImGui PUBLIC SDL3::SDL3)
    else()
        message(FATAL_ERROR "ImGui requires an SDL3 target, but none was found.")
    endif()

    target_compile_definitions(ImGui PRIVATE
        $<$<PLATFORM_ID:Windows>:_CRT_SECURE_NO_WARNINGS>
    )

    set_target_properties(ImGui PROPERTIES FOLDER "Dependencies")
endif()

# ImGuizmo - 3D gizmo for ImGui
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo)
    message(STATUS "Adding ImGuizmo library (custom build)")
    file(GLOB IMGUIZMO_SOURCES 
        ImGuizmo/ImGuizmo.cpp
    )
    
    if(IMGUIZMO_SOURCES)
        add_library(ImGuizmo STATIC ${IMGUIZMO_SOURCES})
        target_include_directories(ImGuizmo PUBLIC ImGuizmo)
        
        # Link with ImGui if available
        if(TARGET ImGui)
            target_link_libraries(ImGuizmo PUBLIC ImGui)
        endif()
        
        # Disable precompiled headers for ImGuizmo
        set_target_properties(ImGuizmo PROPERTIES 
            DISABLE_PRECOMPILE_HEADERS ON
            FOLDER "Dependencies"
        )
    endif()
endif()

# Lua - Scripting language
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lua)
    message(STATUS "Adding Lua library (custom build)")
    
    # Try to find Lua sources in different possible locations
    set(LUA_SOURCE_DIRS 
        lua/src
        lua/lua
        lua
    )
    
    foreach(dir ${LUA_SOURCE_DIRS})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir})
            file(GLOB LUA_SOURCES ${dir}/*.c ${dir}/*.h)
            if(LUA_SOURCES)
                # Remove lua.c and luac.c as they contain main functions
                list(FILTER LUA_SOURCES EXCLUDE REGEX ".*lua\\.c$")
                list(FILTER LUA_SOURCES EXCLUDE REGEX ".*luac\\.c$")
                
                add_library(Lua STATIC ${LUA_SOURCES})
                target_include_directories(Lua PUBLIC ${dir})
                
                if(MSVC)
                    target_compile_definitions(Lua PRIVATE _CRT_SECURE_NO_WARNINGS)
                endif()
                
                set_target_properties(Lua PROPERTIES FOLDER "Dependencies")
                break()
            endif()
        endif()
    endforeach()
endif()

# Sol2 - C++ Lua binding (header-only)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sol/include OR EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sol/single/include)
    message(STATUS "Adding Sol2 library (header-only)")
    add_library(sol INTERFACE)
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sol/include)
        target_include_directories(sol INTERFACE sol/include)
    elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sol/single/include)
        target_include_directories(sol INTERFACE sol/single/include)
    endif()
    
    # Link with Lua if available
    if(TARGET Lua)
        target_link_libraries(sol INTERFACE Lua)
    endif()
endif()

# EnTT - Entity Component System (header-only)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/entt/include OR EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/entt/single_include)
    message(STATUS "Adding EnTT library (header-only)")
    add_library(entt INTERFACE)
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/entt/include)
        target_include_directories(entt INTERFACE entt/include)
    elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/entt/single_include)
        target_include_directories(entt INTERFACE entt/single_include)
    endif()
endif()

# Nlohmann Json parser - Entity Component System (header-only)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann/include OR EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann/single_include)
    message(STATUS "Adding Nlohmann json parser library (header-only)")
    add_library(nlohmann INTERFACE)
    
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann/include)
        target_include_directories(nlohmann INTERFACE nlohmann/include)
    elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann/single_include)
        target_include_directories(nlohmann INTERFACE nlohmann/single_include)
    endif()
endif()

# STB libraries - Header-only imaging libraries
# STB libraries - Header-only imaging libraries
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
    message(STATUS "Adding STB Image library (header-only)")
    add_library(stb_image INTERFACE)

    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/include/stb_image.h)
        target_include_directories(stb_image INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/include
        )
    elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/stb_image.h)
        target_include_directories(stb_image INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/stb_image
        )
    else()
        message(FATAL_ERROR "stb_image.h not found in stb_image/include or stb_image/")
    endif()

    set_target_properties(stb_image PROPERTIES FOLDER "Dependencies")
endif()

# if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
#     message(STATUS "Adding STB Image library (header-only)")
#     add_library(stb_image INTERFACE)
#     target_include_directories(stb_image INTERFACE)
#      if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/include)
#         target_include_directories(ImGui PUBLIC stb_image/include)
#     endif()
#     if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
#         target_include_directories(ImGui PUBLIC stb_image)
#     endif()
# endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stb_truetype)
    message(STATUS "Adding STB TrueType library (header-only)")
    add_library(stb_truetype INTERFACE)
    target_include_directories(stb_truetype INTERFACE stb_truetype)
endif()

# Print summary of found libraries
message(STATUS "")
message(STATUS "=== Library Summary ===")
get_property(ALL_TARGETS DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
foreach(target ${ALL_TARGETS})
    get_target_property(target_type ${target} TYPE)
    if(NOT target_type STREQUAL "UTILITY")
        message(STATUS "  ${target} (${target_type})")
    endif()
endforeach()
message(STATUS "========================")
message(STATUS "")