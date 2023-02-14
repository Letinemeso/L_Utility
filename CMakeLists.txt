cmake_minimum_required(VERSION 3.10)

project(Utility)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(Utility STATIC
    source/L_Debug/L_Debug.cpp
    
    source/Stuff/Stopwatch.cpp
    
    source/Stuff/Stopwatch.cpp
)

target_include_directories(Utility PUBLIC 
                            "${PROJECT_BINARY_DIR}" 
                            "${PROJECT_SOURCE_DIR}/include"
)
