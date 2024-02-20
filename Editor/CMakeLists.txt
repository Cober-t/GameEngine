project ( Editor )

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable( Editor ${MY_SOURCES})

target_include_directories( Engine
    PUBLIC 
        include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/glm
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/glad/include
)


target_link_libraries( Editor PRIVATE Engine )

