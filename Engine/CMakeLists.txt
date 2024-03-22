project( Engine )

set( GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/glad  ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/glad)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/spdlog ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/spdlog)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/glfw ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/glfw)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/glm ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/glm)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/stb_image  ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/stb_image)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/stb_truetype ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/stb_truetype)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/raudio ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/raudio)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/imgui ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/imgui)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/entt  ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/entt)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/sol ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/sol)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/lua ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/lua)
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/box2d ${CMAKE_CURRENT_SOURCE_DIR}/../bin/Engine/box2d)


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library( Engine STATIC ${MY_SOURCES})


target_include_directories( Engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        include
        include/Core
        include/Render
        include/Platforms
        include/ImGui
)

target_link_libraries(Engine PUBLIC 
	glm glfw glad spdlog stb_image stb_truetype raudio imgui entt sol lua box2d)

target_precompile_headers( Engine PUBLIC pch.h )

