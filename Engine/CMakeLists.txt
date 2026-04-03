project(Engine)

# Collect all source files
file(GLOB_RECURSE ENGINE_SOURCES 
    "src/*.cpp"
    "include/*.h"
)

# Add stb_image sources
file(GLOB STB_IMAGE_SOURCES 
    "${CMAKE_SOURCE_DIR}/libraries/stb_image/*.cpp"
    "${CMAKE_SOURCE_DIR}/libraries/stb_image/*.h"
)

# Add GLM sources
file(GLOB_RECURSE GLM_SOURCES 
    "${CMAKE_SOURCE_DIR}/libraries/glm/glm/*.hpp"
    "${CMAKE_SOURCE_DIR}/libraries/glm/glm/*.inl"
)

# Create the engine library as shared (DLL)
add_library(${PROJECT_NAME} STATIC
    ${ENGINE_SOURCES}
    ${STB_IMAGE_SOURCES}
    ${GLM_SOURCES}
)

# Set up precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h>"
)

# Define engine-specific macros
target_compile_definitions(${PROJECT_NAME} PRIVATE
    _CB_BUILD_DLL
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
        ${SDL3_INCLUDE_DIR}
        ${IMGUI_INCLUDE_DIR}
        ${BOX2D_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${ENTT_INCLUDE_DIR}
        ${MSDF_INCLUDE_DIR}
        ${MSDFGEN_INCLUDE_DIR}
        ${STB_IMAGE_INCLUDE_DIR}
        ${STB_TRUETYPE_INCLUDE_DIR}
        ${SPDLOG_INCLUDE_DIR}
        ${LUA_INCLUDE_DIR}
        ${SOL_INCLUDE_DIR}
        ${JSON_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/libraries
)

# Link libraries - using the actual target names from the submodules
target_link_libraries(${PROJECT_NAME} PUBLIC
# Libraries with their own CMake
    $<$<TARGET_EXISTS:SDL3-static>:SDL3-static>
    $<$<TARGET_EXISTS:SDL3::SDL3>:SDL3::SDL3>
    $<$<TARGET_EXISTS:box2d>:box2d>
    $<$<TARGET_EXISTS:spdlog>:spdlog>
    $<$<TARGET_EXISTS:glm>:glm>
    $<$<TARGET_EXISTS:msdf-atlas-gen>:msdf-atlas-gen>
    # Custom built libraries
    $<$<TARGET_EXISTS:ImGui>:ImGui>
    $<$<TARGET_EXISTS:Lua>:Lua>
)

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

# Enable AddressSanitizer for Engine
enable_address_sanitizer(${PROJECT_NAME})

# Configuration-specific settings
# target_compile_options(Engine PRIVATE
#     $<$<CONFIG:Debug>:$<IF:$<CXX_COMPILER_ID:MSVC>,/Od /ZI,>>
#     $<$<CONFIG:Release>:$<IF:$<CXX_COMPILER_ID:MSVC>,/O2,>>
#     $<$<CONFIG:Dist>:$<IF:$<CXX_COMPILER_ID:MSVC>,/O2,>>
# )

# Debug symbols
# set_target_properties(Engine PROPERTIES
#     $<$<CONFIG:Debug>:CMAKE_CXX_FLAGS_DEBUG>
#     $<$<CONFIG:Debug>:"/Zi">
# )