project ( Game )

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable( ${PROJECT_NAME} ${MY_SOURCES})

target_include_directories( ${PROJECT_NAME}
    PUBLIC 
        include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/glm
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/glad/include
)

target_link_libraries( ${PROJECT_NAME} PRIVATE Engine )
