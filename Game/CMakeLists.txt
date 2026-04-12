project(Game)

file(GLOB_RECURSE GAME_SOURCES
    "Game/*.cpp"
    "Game/*.h"
)

add_executable(Game ${GAME_SOURCES})

target_include_directories(Game 
    PRIVATE
        .
        Game
        ${CMAKE_SOURCE_DIR}/Engine/include
        ${CMAKE_SOURCE_DIR}/libraries
        ${GLM_INCLUDE_DIR}
        ${ENTT_INCLUDE_DIR}
        ${IMGUI_INCLUDE_DIR}
        ${BOX2D_INCLUDE_DIR}
        ${SDL3_INCLUDE_DIR}
        ${SDL3SHADERCROSS_INCLUDE_DIR}
        ${STB_IMAGE_INCLUDE_DIR}
        ${STB_TRUETYPE_INCLUDE_DIR}
        ${LUA_INCLUDE_DIR}
        ${SOL_INCLUDE_DIR}
        ${SPDLOG_INCLUDE_DIR}
)

target_link_libraries(Game 
    PRIVATE
        Engine
        $<$<TARGET_EXISTS:SDL3-static>:SDL3-static>
        $<$<TARGET_EXISTS:SDL3::SDL3>:SDL3::SDL3>
        $<$<TARGET_EXISTS:box2d>:box2d>
)


if(WIN32)
    target_link_libraries(Game PRIVATE opengl32)
endif()

target_compile_definitions(Game PRIVATE
    SOLUTION_DIR="${CMAKE_SOURCE_DIR}/"
)

set_target_properties(Game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
)

enable_address_sanitizer(Game)
add_dependencies(Game Engine)