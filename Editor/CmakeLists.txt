project(Editor)

# Collect all source files
file(GLOB_RECURSE EDITOR_SOURCES 
    "src/*.cpp"
    "include/*.h"
)

# Create the editor executable
add_executable(${PROJECT_NAME}  ${EDITOR_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE
        include
        include/Panels
        ${CMAKE_SOURCE_DIR}/Engine/include
        ${CMAKE_SOURCE_DIR}/libraries
        ${GLM_INCLUDE_DIR}
        ${ENTT_INCLUDE_DIR}
        ${IMGUI_INCLUDE_DIR}
        ${IMGUIZMO_INCLUDE_DIR}
        ${BOX2D_INCLUDE_DIR}
        ${SDL3_INCLUDE_DIR}
        ${STB_IMAGE_INCLUDE_DIR}
        ${STB_TRUETYPE_INCLUDE_DIR}
        ${LUA_INCLUDE_DIR}
        ${SOL_INCLUDE_DIR}
        ${SPDLOG_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        Engine
        $<$<TARGET_EXISTS:ImGui>:ImGui>
        $<$<TARGET_EXISTS:ImGuizmo>:ImGuizmo>
        $<$<TARGET_EXISTS:SDL3-static>:SDL3-static>
        $<$<TARGET_EXISTS:SDL3::SDL3>:SDL3::SDL3>
        $<$<TARGET_EXISTS:box2d>:box2d>
)

# Windows-specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif()

# Editor-specific definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    __EDITOR__
    SOLUTION_DIR="${CMAKE_SOURCE_DIR}/"
    EDITOR_ICONS_DIR="${CMAKE_SOURCE_DIR}/Editor/icons/"
)

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>
)
    
# Enable AddressSanitizer for Editor
enable_address_sanitizer(${PROJECT_NAME})
# Make Editor depend on Engine to ensure proper build order
add_dependencies(${PROJECT_NAME} Engine)

# # Set folder in Visual Studio
# set_target_properties(Editor PROPERTIES FOLDER "Tools")

# # Configuration-specific settings
# target_compile_options(Editor PRIVATE
#     $<$<CONFIG:Debug>:$<IF:$<CXX_COMPILER_ID:MSVC>,/MDd,>>
#     $<$<CONFIG:Release>:$<IF:$<CXX_COMPILER_ID:MSVC>,/MD,>>
#     $<$<CONFIG:Dist>:$<IF:$<CXX_COMPILER_ID:MSVC>,/MD,>>
# )

# # Debug symbols for debug configuration
# set_target_properties(Editor PROPERTIES
#     $<$<CONFIG:Debug>:CMAKE_CXX_FLAGS_DEBUG>
#     $<$<CONFIG:Debug>:"/Zi">
# )
