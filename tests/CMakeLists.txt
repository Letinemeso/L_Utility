file(GLOB_RECURSE Sources "./*.cpp")

if(MSVC)
    add_compile_options(/bigobj)
endif(MSVC)

add_executable(L_Utility_Tests ${Sources})

target_include_directories(L_Utility_Tests PUBLIC
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/L_Utility/include"
)


target_link_libraries(L_Utility_Tests PUBLIC
    L_Utility
)

if (UNIX)
    target_link_libraries(L_Utility_Tests PUBLIC
        pthread
    )
endif (UNIX)
