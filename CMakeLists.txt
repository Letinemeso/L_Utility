cmake_minimum_required(VERSION 3.5)

project(L_Utility LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB_RECURSE LibFiles "include/*.h")
add_custom_target(L_Utility_include SOURCES ${LibFiles})

add_library(L_Utility STATIC
    source/Stuff/File.cpp
    source/Stuff/Stopwatch.cpp
    source/Stuff/Timer.cpp
    source/L_Debug/L_Debug.cpp
)

target_include_directories(L_Utility PUBLIC 
                            "${PROJECT_BINARY_DIR}" 
                            "${PROJECT_SOURCE_DIR}/include"
)
