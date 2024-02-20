cmake_minimum_required( VERSION 3.16 )


set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin/Editor)

project( Editor )

add_subdirectory( Engine )
add_subdirectory( Editor )