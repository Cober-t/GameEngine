cmake_minimum_required( VERSION 3.16 )

project( Game )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin/${PROJECT_NAME})


# This is useful to get an ASSETS_PATH in your IDE during development 
# but you should comment this if you compile a release version and uncomment the next line
add_compile_definitions(PUBLIC SCENES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/scenes/")
add_compile_definitions(PUBLIC SHADERS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/")
add_compile_definitions(PUBLIC SCRIPTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/scripts/")
add_compile_definitions(PUBLIC ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
#add_compile_definitions(PUBLIC __EDITOR__)

# Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the project
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")


add_subdirectory( Engine )
add_subdirectory( ${PROJECT_NAME} )